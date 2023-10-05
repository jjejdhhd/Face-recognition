笔记主要参考B站视频“[【C语言项目】软件开发：人脸识别](https://www.bilibili.com/video/BV18R4y1K7bH)”。

# 1. “项目笔记”

跟随频频一步步实现时，做的较为详尽的笔记。里面给出了素材链接。


# 2. “快速查看源码”

存储了关键的代码文件，而忽略了MFC的框架等无关紧要的自动生成的固定代码。下面是这些代码的说明：
> 外部添加代码
> 1. ButtonPNG：用于美化按钮的显示。
> 2. faceTool：使用虹软人脸识别SDK完成人脸识别功能。
> 3. VideoPlayer：使用vlc的SDK完成视频的播放、暂停、退出等。
>
> 剩下的都是窗口的“添加类”：
> 1. face_recognition：整个项目的主函数，自动生成。
> 2. face_recognitionDlg：“启动窗口”的函数，自动生成。
> 3. WinLogin：“登录窗口”的函数，“添加类”生成主体。
> 4. WinRegister：“注册窗口”的函数，“添加类”生成主体。
> 5. WinWelcome：“欢迎窗口”的函数，“添加类”生成主体。
> 6. WinHome：“HOME窗口的函数”，“添加类”生成主体。
>
> 其余的代码文件暂时不需要了解太多。

# 3. 项目工程

> - [1-刚添加完人脸识别框.rar](https://pan.baidu.com/s/1al1qtjeWG0FxWOFVaoNnNA?pwd=7f8h)：完成“项目笔记-第14节”。
> - [2-流浪地球风格美化完成.rar](https://pan.baidu.com/s/1yPnYLpoVan3dDJISm3Bqsg?pwd=tinr)：完成“项目笔记-后记1”。
>
> 环境：Visual Studio 2022
> 依赖1-OpenCV4.8.0：可以去[OpenCV官网]()自行下载并解压，可以参考“项目笔记-第7节”。我将其解压后放在了D:\opencv480\。
> 依赖2-虹软SDK：需要自行去虹软官网注册并下载[人脸识别SDK](https://ai.arcsoft.com.cn/product/arcface.html)来获取APP_ID、SDK_KEY，并在“faceTool.h”文件中更改，可以参考“项目笔记-第9节”。
> 依赖3-视频：由于视频播放需要绝对路径，所以需要在WinHome.cpp最后的按钮点击函数 WinHome::OnBnClickedHomePlayPause() 中更改路径。
> 其他：编译运行，看还报啥错就Debug好了。

# 4. 项目评价

1. 项目本身。还挺有意思的，Rock本身讲解的功能也很多，所以我的优化仅仅局限在使用最新(2023年9月)的OpenCV4.8.0和美工上了。
2. 摄像头显示。两个思路，一个是使用CvvImage(OpenCV2.x时代的老代码，暂时没能力重构)，一个是将OpenCV窗口嵌入到图片控件中(偷懒但简单，我的思路)。第二个思路会有个缺点，就是首先创建OpenCV窗口在嵌入到图片控件之前，会显示出来，所以程序效果就是开启摄像头的时候，会有一个多余的窗口一闪而过。后续有时间深入了解一下窗口句柄、图片格式等，可能会试试重构来解决这个问题。





