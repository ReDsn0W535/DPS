#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <dps.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent * event)
    {
        dps::DPS m;
        std::vector< std::pair<dps::point, int> > points;
        m.readPoints(&points);
        QPainter p(this);
        QPen pen;
        pen.setColor(QColor(Qt::black));
        pen.setWidth(3);
        p.setPen(pen);
        for (auto& i : points){
            p.drawPoint(i.first.x, i.first.y);
        }
        points = m.startDPS();
        pen.setColor(QColor(Qt::red));
        p.setPen(pen);
        for (auto& i : points){
            p.drawPoint(i.first.x, i.first.y);
        }
    }
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
