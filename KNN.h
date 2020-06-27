#ifndef KNN_H_INCLUDED
#define KNN_H_INCLUDED

#include "NumberRecogniser.h"

class KNN : public NumberRecogniser
{
public:
	KNN();
	~KNN();
	bool train(const string imagesFilePath, const string labelsFilePath) override;
	float test(const string imagesFilePath, const string labelsFilePath) override;
	vector<int> identifyNumbers(Mat image) override;
private:
	Ptr<ml::KNearest> knn;
};

#endif //KNN_H_INCLUDED