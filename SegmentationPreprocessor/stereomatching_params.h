#ifndef STEREOMATCHING_PARAMS_H
#define STEREOMATCHING_PARAMS_H

#include <QWidget>

namespace Ui {
class StereoMatching_params;
}

class StereoMatching_params : public QWidget
{
    Q_OBJECT

public:
    explicit StereoMatching_params(QWidget *parent = 0);
    ~StereoMatching_params();

    std::vector<float> getParams();

private:
    Ui::StereoMatching_params *ui;
};

#endif // STEREOMATCHING_PARAMS_H
