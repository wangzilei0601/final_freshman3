#include <QCoreApplication>
#include <QDebug>
#include "Track.h"
#include "Playlist.h"

// 辅助函数：打印当前播放状态
void printStatus(const Playlist &pl) {
    Track current = pl.currentTrack();
    qDebug() << "=========================================";
    qDebug() << "当前索引:" << pl.currentIndex() 
             << " | 总曲目:" << pl.count();
    if (!current.filePath().isEmpty()) {
        qDebug() << "正在播放: " << current.title() 
                 << " - " << current.artist();
    } else {
        qDebug() << "当前无歌曲播放";
    }
    qDebug() << "播放模式: " << pl.playModeString();
    qDebug() << "=========================================";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 1. 创建测试数据
    Track t1("D:/Music/青花瓷.mp3");
    t1.setTitle("青花瓷");
    t1.setArtist("周杰伦");

    Track t2("D:/Music/稻香.mp3");
    t2.setTitle("稻香");
    t2.setArtist("周杰伦");

    Track t3("D:/Music/夜曲.mp3");
    t3.setTitle("夜曲");
    t3.setArtist("周杰伦");

    Track t4("D:/Music/晴天.mp3");
    t4.setTitle("晴天");
    t4.setArtist("周杰伦");

    // 2. 创建播放列表并添加歌曲
    Playlist pl;
    pl.addTrack(t1);
    pl.addTrack(t2);
    pl.addTrack(t3);
    
    qDebug() << "【测试 1：初始状态】";
    printStatus(pl); // 预期：索引 0，青花瓷

    // 3. 测试插入
    qDebug() << "\n【测试 2：插入歌曲到中间（索引 1）】";
    pl.insertTrack(1, t4);
    printStatus(pl); // 索引应该还是 0（青花瓷），因为插入在它后面

    // 4. 测试切换下一首（列表循环模式）
    qDebug() << "\n【测试 3：下一首（列表循环）】";
    Track next = pl.next();
    qDebug() << "切换到: " << next.title(); // 预期：稻香
    printStatus(pl); // 索引应为 1

    next = pl.next();
    qDebug() << "切换到: " << next.title(); // 预期：夜曲
    printStatus(pl); // 索引应为 2

    // 5. 测试上一首（基于历史）
    qDebug() << "\n【测试 4：上一首（回到历史）】";
    Track prev = pl.prev();
    qDebug() << "回退到: " << prev.title(); // 预期：稻香
    printStatus(pl); // 索引应为 1

    // 6. 测试单曲循环
    qDebug() << "\n【测试 5：单曲循环模式】";
    pl.setPlayMode(Playlist::PlayMode::SingleMode);
    qDebug() << "当前模式: " << pl.playModeString();
    Track singleNext = pl.next();
    qDebug() << "单曲循环下一首: " << singleNext.title(); // 预期：稻香（不变）
    printStatus(pl); // 索引不变，还是 1

    // 7. 测试随机模式
    qDebug() << "\n【测试 6：随机模式（理论上会随机跳）】";
    pl.setPlayMode(Playlist::PlayMode::RandomMode);
    for (int i = 0; i < 3; ++i) {
        Track randTrack = pl.next();
        qDebug() << "随机切到: " << randTrack.title();
    }
    printStatus(pl);

    // 8. 测试删除当前歌曲（删除后应自动播放下一首）
    qDebug() << "\n【测试 7：删除当前正在播放的歌曲】";
    int curIdx = pl.currentIndex();
    qDebug() << "即将删除索引: " << curIdx << " 的歌";
    pl.removeTrack(curIdx);
    printStatus(pl); // 索引应指向原来的下一首（或最后一首）

    // 9. 测试清空
    qDebug() << "\n【测试 8：清空列表】";
    pl.clear();
    printStatus(pl); // 应显示无歌曲

    // 防止控制台一闪而过（如果跑在 GUI 模式下则不需要这行，但为了安全保留）
    qDebug() << "\n测试完成。按任意键退出...";

    return 0; // 正常退出
}