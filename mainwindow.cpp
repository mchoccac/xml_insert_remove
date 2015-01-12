#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QString>

int Item::maxSN = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    document = new QDomDocument("sample");
    QFile file(":/sample.xml");
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!document->setContent(&file)) {
        file.close();
        return;
    }
    file.close();
    qDebug() << document->childNodes().count();
    refreshItemComboBox();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshItemComboBox()
{
    ui->nodeListComboBox->clear();
    QDomNode items = document->namedItem("items");
    for(int i = 0; i< items.childNodes().size(); i++)
    {
        QDomNode child = items.childNodes().at(i);
        QString s_no = child.attributes().namedItem("s_no").nodeValue();
        qDebug() << s_no;
        ui->nodeListComboBox->addItem(s_no);
    }
}


Item::Item(QDomNode node)
{
    if(true)
    {
//        qDebug() << node.nodeName() << node.hasChildNodes() << node.hasAttributes();
//        qDebug() << node.attributes().size() << node.childNodes().size();
        for(int i = 0; i < node.attributes().size(); i++)
        {
            QDomAttr attr = node.attributes().item(i).toAttr();
            if(attr.name() == "s_no")
            {
                this->s_no = attr.value();
            }
            else
            {
                qDebug() << "unsupported attribute!";
            }
        }
        for(int i = 0; i < node.childNodes().size(); i++)
        {
            QDomNode child = node.childNodes().at(i);
            if(child.nodeName() == "title")
            {
                title = child.toElement().text();
            }
            else if(child.nodeName() == "path")
            {
                path = child.toElement().text();
            }
            else if(child.nodeName() == "desc")
            {
                desc = child.toElement().text();
            }
            else
            {
                qDebug() << "Unsupported child node found:" << child.nodeName();
            }
        }
    }
    else
    {
        s_no = node.attributes().namedItem("s_no").nodeValue();
        title = node.namedItem("title").toElement().text();
        path = node.namedItem("path").toElement().text();
        desc = node.namedItem("desc").toElement().text();
    }
    if(s_no.toInt() > maxSN)
        maxSN = s_no.toInt();
}

QDomNode Item::toNode(QDomDocument doc)
{
    QDomElement node = doc.createElement("item");
    node.setAttribute("s_no", s_no);
    QDomElement t = doc.createElement("title");
    t.appendChild(doc.createTextNode(title));
    QDomElement p = doc.createElement("path");
    p.appendChild(doc.createTextNode(path));
    QDomElement d = doc.createElement("desc");
    d.appendChild(doc.createTextNode(desc));
    node.appendChild(t);
    node.appendChild(p);
    node.appendChild(d);
    return node;
}

QString Item::toString()
{
    QString str;
    str += "title:" + title;
    str += "\npath:" + path;
    str += "\ndesc:" + desc;
    return str;
}

void MainWindow::on_nodeListComboBox_currentIndexChanged(const QString &arg1)
{
    QDomNode items = document->namedItem("items");
    for(int i = 0; i< items.childNodes().size(); i++)
    {
        QDomNode child = items.childNodes().at(i);
        QString s_no = child.attributes().namedItem("s_no").nodeValue();
        Item item(child);
        if(s_no == arg1)
        {
            ui->nodePreviewTextEdit->setText(item.toString());
        }
    }
}

void MainWindow::on_insertNodeButton_clicked()
{
    QDomNode items = document->namedItem("items");
    for(int i = 0; i< items.childNodes().size(); i++)
    {
        QDomNode child = items.childNodes().at(i);
        QString s_no = child.attributes().namedItem("s_no").nodeValue();
        if(s_no == ui->nodeListComboBox->currentText())
        {
            Item item;
            int sn = item.maxSN + 1;
            item.s_no = QString::number(sn);
            item.maxSN = sn;
            item.title = "title_" + item.s_no;
            item.path = "path_" + item.s_no;
            item.desc = "desc_" + item.s_no;
            QDomNode retVal = items.insertAfter(item.toNode(*document), child);
            if(retVal.isNull())
                qDebug() << "failed to insert item!";
            break;
        }
    }
    qDebug() << items.childNodes().size();
    refreshItemComboBox();
}

void MainWindow::on_deleteNode_clicked()
{
    QDomNode items = document->namedItem("items");
    for(int i = 0; i< items.childNodes().size(); i++)
    {
        QDomNode child = items.childNodes().at(i);
        QString s_no = child.attributes().namedItem("s_no").nodeValue();
        if(s_no == ui->nodeListComboBox->currentText())
        {
            items.removeChild(child);
            break;
        }
    }
    qDebug() << items.childNodes().size();
    refreshItemComboBox();
}
