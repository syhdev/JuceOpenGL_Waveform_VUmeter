//
// Created by Sylvain on 27/04/2020.
//

#ifndef RECORDER_CIRCULARBUFFER_H
#define RECORDER_CIRCULARBUFFER_H

#include <sys/types.h>
#include <memory>


template<class T> //T should be int16_t or float (audio data type)
struct SamplesBuffer {
	int32_t numSamples;
	T* buffer;
};


template<class T> //T should be int16_t or float (audio data type)
class GraphicsCircularBuffer {
public:
	GraphicsCircularBuffer(size_t numBuffer) :
		mCircBuf(std::unique_ptr<SamplesBuffer<T>[]>(new SamplesBuffer<T>[numBuffer])),
		mMaxNumBuffer(numBuffer) {
		for (int i = 0; i < numBuffer; i++) {
			mCircBuf[i].buffer = new T[4096]; // Usually, numSample is 48, 96, 192 so we should have enough space
		}
	}

	void put(const T* data, int32_t numSample) {

		// Check it otherwise we will have an array overflow
		jassert(numSample <= 4096);

		if (mFull.load()) {
			mTail++;
			if (mTail.load() >= mMaxNumBuffer) {
				mTail.fetch_sub(mMaxNumBuffer);
			}
		}

		memcpy(mCircBuf[mHead].buffer, data, numSample * sizeof(T));

		mCircBuf[mHead].numSamples = numSample;

		mHead++;
		if (mHead.load() >= mMaxNumBuffer) {
			mHead.fetch_sub(mMaxNumBuffer);
		}

		mFull.store(mHead.load() == mTail.load());

		putCount = putCount + numSample;

	}

	SamplesBuffer<T> get() {


		SamplesBuffer<T> val = mCircBuf[mTail.load()];
		getCount = getCount + (mCircBuf[mTail.load()].numSamples);

		mFull.store(false);

		mTail++;

		if (mTail.load() >= mMaxNumBuffer) {
			mTail.fetch_sub(mMaxNumBuffer);
		}

		return val;
	}

	void reset() {
		mTail.store(0);
		mHead.store(0);
		mFull.store(false);
	}

	bool isEmpty() const {
		return (!mFull.load() && (mHead.load() == mTail.load()));
	}

	int putCount = 0;
	int getCount = 0;
	int fullCount = 0;
	int emptyCount = 0;

private:

	std::unique_ptr<SamplesBuffer<T>[]> mCircBuf;

	std::atomic_size_t mHead{ 0 };
	std::atomic_size_t mTail{ 0 };

	const size_t mMaxNumBuffer;
	std::atomic<bool> mFull{ false };

};




#endif //RECORDER_CIRCULARBUFFER_H
