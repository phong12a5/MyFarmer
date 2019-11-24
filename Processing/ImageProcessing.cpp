#include "ImageProcessing.hpp"
#include "Model.hpp"

#define MODEL Model::instance()

ImageProcessing::ImageProcessing(QObject *parent) : QObject(parent)
{

}
QPoint ImageProcessing::findImageOnImage(const QString &smallImagePath, const QString &largeImagePath)
{
    QPoint retVal;

//    cv::Mat _smallImage = ImageProcessing::QImage2Mat(QImage(smallImagePath));
//    cv::Mat _largeImage = ImageProcessing::QImage2Mat(QImage(largeImagePath));
#ifdef ANDROID_KIT
    cv::Mat _smallImage = cv::imread(smallImagePath.toUtf8().constData(),CV_LOAD_IMAGE_COLOR);
    cv::Mat _largeImage = cv::imread(largeImagePath.toUtf8().constData(),CV_LOAD_IMAGE_COLOR);
#else
    cv::Mat _smallImage = cv::imread(smallImagePath.toUtf8().constData(),1);
    cv::Mat _largeImage = cv::imread(largeImagePath.toUtf8().constData(),1);
#endif

    LOG << "_smallImage: " << QImage(smallImagePath).size();
    LOG << "largeImagePath: " << QImage(largeImagePath).size();

    // Resize image
    float threshold = ImageProcessing::getThreshhold();
    float scale = ImageProcessing::getScale();

    if(scale != 1){
         cv::Mat _smallImageCoppy = _smallImage;
        cv::resize(_smallImageCoppy,_smallImage,cv::Size(),scale,scale);
    }

    //kiểm tra kích cỡ của ảnh input & template
    if (_smallImage.rows > _largeImage.rows || _smallImage.cols > _largeImage.cols)
    {
        LOG << "Mat template must be smaller than matInput";
        return retVal;
    }else if(_smallImage.rows <= 0 || _smallImage.cols <= 0 || _largeImage.rows <= 0 || _largeImage.cols <= 0){
        LOG << "Invalid Image";
        return retVal;
    }

    cv::Mat result;
    int result_cols = _largeImage.cols - _smallImage.cols + 1;
    int result_rows = _largeImage.rows - _smallImage.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);

    //tìm mẫu
//    cv::matchTemplate(_largeImage, _smallImage, result, CV_TM_CCORR_NORMED);
    cv::matchTemplate(_largeImage, _smallImage, result, CV_TM_CCOEFF_NORMED);

    cv::threshold(result, result, threshold, 1., CV_THRESH_TOZERO);
    double minval, maxval;
    double bestMaxval = 0;
    //ngưỡng chính xác, giảm xuống thì sẽ tìm được nhiều đối tượng hơn nhưng sai số nhiều hơn

    while (true)
    {
        cv::Point minloc, maxloc;
        cv::minMaxLoc(result, &minval, &maxval, &minloc, &maxloc);

        if (maxval > threshold)
        {
            //vẽ hình chữ nhật lên đối tượng tìm được
            if(maxval > bestMaxval){
                bestMaxval = maxval;
                retVal = QPoint(maxloc.x + _smallImage.cols/2, maxloc.y + _smallImage.rows/2);
            }
            cv::floodFill(result, maxloc, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));

        }
        else
            break;
    }
    LOG << smallImagePath.split("/").last() << " : " << retVal << QString(" %1--%2").arg(threshold).arg(scale);
    return retVal;
}

QPoint ImageProcessing::findImageOnImage(const QString &templatePath, cv::Mat &screenImg)
{
    QPoint retVal;

    if(screenImg.empty()){
        LOG << "screenImg is EMPTY";
        return retVal;
    }

#ifdef ANDROID_KIT
    cv::Mat templateImg = cv::imread(templatePath.toUtf8().constData(),CV_LOAD_IMAGE_COLOR);
#else
    cv::Mat _smallImage = cv::imread(smallImagePath.toUtf8().constData(),1);
    cv::Mat _largeImage = cv::imread(largeImagePath.toUtf8().constData(),1);
#endif

    // Resize image
    float threshold = ImageProcessing::getThreshhold();
    float scale = ImageProcessing::getScale();

    if(scale != 1){
         cv::Mat templateImgCoppy = templateImg;
//         LOG << "templateImg.rows: " << templateImg.rows;

        cv::resize(templateImgCoppy,templateImg,cv::Size(),scale,scale);
//        LOG << "templateImg.rows: " << templateImg.rows;
//        LOG << "templateImgCoppy.rows: " << templateImgCoppy.rows;
    }

    //kiểm tra kích cỡ của ảnh input & template
    if (templateImg.rows > screenImg.rows || templateImg.cols > screenImg.cols)
    {
        LOG << "Mat template must be smaller than matInput";
        return retVal;
    }else if(templateImg.rows <= 0 || templateImg.cols <= 0 || screenImg.rows <= 0 || screenImg.cols <= 0){
        LOG << "Invalid Image";
        return retVal;
    }

    cv::Mat result;
    int result_cols = screenImg.cols - templateImg.cols + 1;
    int result_rows = screenImg.rows - templateImg.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);

    //tìm mẫu
//    cv::matchTemplate(_largeImage, templateImg, result, CV_TM_CCORR_NORMED);
    cv::matchTemplate(screenImg, templateImg, result, CV_TM_CCOEFF_NORMED);

    cv::threshold(result, result, threshold, 1., CV_THRESH_TOZERO);
    double minval, maxval;
    double bestMaxval = 0;
    //ngưỡng chính xác, giảm xuống thì sẽ tìm được nhiều đối tượng hơn nhưng sai số nhiều hơn

    while (true)
    {
        cv::Point minloc, maxloc;
        cv::minMaxLoc(result, &minval, &maxval, &minloc, &maxloc);

        if (maxval > threshold)
        {
            //vẽ hình chữ nhật lên đối tượng tìm được
            if(maxval > bestMaxval){
                bestMaxval = maxval;
                retVal = QPoint(maxloc.x + templateImg.cols/2, maxloc.y + templateImg.rows/2);
            }
            cv::floodFill(result, maxloc, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));

        }
        else
            break;
    }
//    LOG << smallImagePath.split("/").last() << " : " << retVal << QString(" %1--%2").arg(threshold).arg(scale);
    return retVal;
}

float ImageProcessing::getThreshhold()
{
#ifdef ANDROID_KIT
//    float dpi = MODEL->deviceInfo().disInfo.dpi;
//    if (dpi<160)
//        return 0.83;
//    else if (dpi<213)
//        return 0.83;
//    else if (dpi<320)
//        return 0.8;
//    else if (dpi<480)
//        return 0.8;
//    else if (dpi<640)
//        return 0.8;
//    else /*if (dpi>=640)*/
        return 0.95;
#else
    return 0.8;
#endif
}

float ImageProcessing::getScale()
{
#ifdef ANDROID_KIT
//    float dpi = MODEL->deviceInfo().disInfo.dpi;
//    if (dpi<160)
//        return 0.263;
//    else if (dpi<213)
//        return 0.3575;
//    else if (dpi<320)
//        return 0.503;
//    else if (dpi<480)
//        return 0.705;
//    else if (dpi<640)
//        return 1;
//    else /*if (dpi>=640)*/
        return 1;
#else
    return 1;
#endif
}
#ifdef ANDROID_KIT

QList<QPoint> ImageProcessing::findImageListOnImage(const QString &smallImagePath, const QString &largeImagePath)
{
    QList<QPoint> retVal;

//    cv::Mat _smallImage = ImageProcessing::QImage2Mat(QImage(smallImagePath));
//    cv::Mat _largeImage = ImageProcessing::QImage2Mat(QImage(largeImagePath));
    cv::Mat _smallImage = cv::imread(smallImagePath.toUtf8().constData(),CV_LOAD_IMAGE_COLOR);
    cv::Mat _largeImage = cv::imread(largeImagePath.toUtf8().constData(),CV_LOAD_IMAGE_COLOR);

    // Resize image
    float threshold = ImageProcessing::getThreshhold();
    float scale = ImageProcessing::getScale();

    if(scale != 1){
         cv::Mat _smallImageCoppy = _smallImage;
         LOG << "_smallImage.rows: " << _smallImage.rows;

        cv::resize(_smallImageCoppy,_smallImage,cv::Size(),scale,scale);
        LOG << "_smallImage.rows: " << _smallImage.rows;
        LOG << "_smallImageCoppy.rows: " << _smallImageCoppy.rows;
    }

    //kiểm tra kích cỡ của ảnh input & template
    if (_smallImage.rows > _largeImage.rows || _smallImage.cols > _largeImage.cols)
    {
        LOG << "Mat template must be smaller than matInput";
        LOG << "Small image: " << QImage(smallImagePath).size();
        LOG << "Large image: " << QImage(largeImagePath).size();
        return retVal;
    }else if(_smallImage.rows <= 0 || _smallImage.cols <= 0 || _largeImage.rows <= 0 || _largeImage.cols <= 0){
        LOG << "Invalid Image";
        LOG << "Small image: " << QImage(smallImagePath).size();
        LOG << "Large image: " << QImage(largeImagePath).size();
        return retVal;
    }

    cv::Mat result;
    int result_cols = _largeImage.cols - _smallImage.cols + 1;
    int result_rows = _largeImage.rows - _smallImage.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);

    //tìm mẫu
//    cv::matchTemplate(_largeImage, _smallImage, result, CV_TM_CCORR_NORMED);
    cv::matchTemplate(_largeImage, _smallImage, result, CV_TM_CCOEFF_NORMED);

    cv::threshold(result, result, threshold, 1., CV_THRESH_TOZERO);
    double minval, maxval;
    //ngưỡng chính xác, giảm xuống thì sẽ tìm được nhiều đối tượng hơn nhưng sai số nhiều hơn

    while (true)
    {
        cv::Point minloc, maxloc;
        cv::minMaxLoc(result, &minval, &maxval, &minloc, &maxloc);

        if (maxval > threshold)
        {
            retVal << QPoint(maxloc.x + _smallImage.cols/2, maxloc.y + _smallImage.rows/2);
            cv::floodFill(result, maxloc, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));
        }
        else
            break;
    }
    LOG << smallImagePath.split("/").last() << " : " << retVal << QString(" %1--%2").arg(threshold).arg(scale);
    return retVal;
}
#endif
