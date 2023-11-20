# 基于OpenCV的智能人脸识别系统

#### 介绍
基于QTcreate软件使用opencv和sqlite3的第三方库，采用C++语言开发的人脸识别系统，
具有人脸录入以及人脸识别功能。
人脸录入：
    1、收集人脸图片                        
    2、人脸检测
    3、提取人脸特征值
    4、把人脸特征值与人名存入数据库
人脸识别：
    1、采集人脸数据
    2、人脸检测
    3、提取人脸特征值
    4、从数据库中匹配人脸特征值，匹配成功返回其人名

#### 软件架构
在window环境下运行，使用QTcreator软件编译，采用C++语句编写，使用sqlite3、OpenCV4.55第三方库；
主要使用OpenCV库中以下
图像处理函数：
    1、加载图像
        cv::Mat img = cv::imread("path");
    2、显示图像
        cv::imshow("image", img);
    3、获取按键(阻塞程序流)
        cv::waitKey();
    4、销毁窗体
        cv::destroyAllWindows();
人脸检测：
    1、人脸检测模型
    std::string fd_model_path = "path"; 
    auto face_detector = cv::FaceDetectorYN::create(fd_model_path, "", img.size());
    face_detector->detect(img, face_pos);
    2、标记人脸
    cv::rectangle(img, cv::Rect(x, y, w, h), cv::Scalar(0,255,255));
    3、提取人脸特征值
    cv::Mat face_feature;
    face_recognizer->feature(align_img, face_feature);
    4、比较人脸特征值
    face_recognizer->match();
使用sqlite3中数据库将提取的人脸特征值与对应的id，姓名存储到数据库中，并可对数据库进行插入、查询、删除操作。


#### 安装教程

    windows系统
    C++语言
    QtCreator
    opencv4.5.5

#### 使用说明

1.  QT安装
2.  OpenCV安装
3.  配置项目环境以及头文件库文件路径

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
