#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDomDocument>
#include <QDomNode>

struct Item
{
    QString title;
    QString path;
    QString desc;
    QString s_no;
    Item(QDomNode);
    Item(){}
    QDomNode toNode(QDomDocument);
    QString toString();
    static int maxSN;
};



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void refreshItemComboBox();

private slots:
    void on_nodeListComboBox_currentIndexChanged(const QString &arg1);
    void on_insertNodeButton_clicked();

    void on_deleteNode_clicked();

private:
    Ui::MainWindow *ui;
    QDomDocument * document;
    QDomNode * currentNode;
};

#endif // MAINWINDOW_H
