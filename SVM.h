#ifndef SVM_H_INCLUDED
#define SVM_H_INCLUDED

#include "NumberRecogniser.h"

class SVM : public NumberRecogniser
{
public:
	SVM();
	~SVM();
	bool train(const string imagesFilePath, const string labelsFilePath) override;
	float test(const string imagesFilePath, const string labelsFilePath) override;
	vector<int> identifyNumbers(Mat image) override;
private:
	Ptr<ml::SVM> svm;
};

#endif //SVM_H_INCLUDED