/*======================================================
    > File Name: filetreeview.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年04月04日 星期六 23时47分08秒
 =======================================================*/

#include<QHBoxLayout>
#include<QString>
#include<QDialog>
#include<QFileInfo>
#include"filetreeview.h"

#include<QByteArray>
#include<iostream>
using namespace std;

//文件类型标志
#define   ROLE_MARK             Qt::UserRole + 1
#define   ROLE_MARK_FOLDER      Qt::UserRole + 2
//#define   ROLE_MARK_FILE        Qt::UserRole + 3

#define   MARK_FOLDER           1  //文件夹标志
#define   MARK_FILE             2  //文件标志

#define   MARK_FOLDER_DOCUMENT      1  //文档文件夹
#define   MARK_FOLDER_PICTURE       2  //图片文件夹
#define   MARK_FOLDER_VIDEO         3  //视频文件夹
#define   MARK_FOLDER_MUSIC         4  //音乐文件夹
#define   MARK_FOLDER_OTHER         5  //其他文件夹

FileWidget :: FileWidget(QWidget *parent) : QWidget(parent)
{
	treeview = new QTreeView(this);
	model = new QStandardItemModel;
	model->setHorizontalHeaderLabels(QStringList()<<tr("我的文件"));

	itemDocument = new QStandardItem(tr("文档"));
	itemDocument->setData(MARK_FOLDER,ROLE_MARK);
	itemDocument->setData(MARK_FOLDER_DOCUMENT,ROLE_MARK_FOLDER);
	model->appendRow(itemDocument);
	//添加一个测试文件
	QStandardItem* test = new QStandardItem("test");
	test->setData(MARK_FILE,ROLE_MARK);
	itemDocument->appendRow(test);

	itemPicture = new QStandardItem(tr("图片"));
	itemPicture->setData(MARK_FOLDER,ROLE_MARK);
	itemPicture->setData(MARK_FOLDER_PICTURE,ROLE_MARK_FOLDER);
	model->appendRow(itemPicture);

	itemVideo = new QStandardItem(tr("视频"));
	itemVideo->setData(MARK_FOLDER,ROLE_MARK);
	itemVideo->setData(MARK_FOLDER_VIDEO,ROLE_MARK_FOLDER);
	model->appendRow(itemVideo);

	itemmusic = new QStandardItem(tr("音乐"));
	itemmusic->setData(MARK_FOLDER,ROLE_MARK);
	itemmusic->setData(MARK_FOLDER_MUSIC,ROLE_MARK_FOLDER);
	model->appendRow(itemmusic);

	itemother = new QStandardItem(tr("其他"));
	itemother->setData(MARK_FOLDER,ROLE_MARK);
	itemother->setData(MARK_FOLDER_OTHER,ROLE_MARK_FOLDER);
	model->appendRow(itemother);
	treeview->setModel(model);

	CreatMenuAction();
	CreatMenu();
    treeview->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(treeview,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(MenuRequested(const QPoint&)));

	QHBoxLayout  *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(treeview);
	setLayout(mainLayout);
}

void  FileWidget::CreatMenuAction()
{
	uploadfile = new QAction(tr("上传"),this);
	downloadfile= new QAction(tr("下载"),this);
}

void  FileWidget::CreatMenu()
{
	folderMenu = new QMenu(this);
	folderMenu->addAction(uploadfile);
	connect(uploadfile,SIGNAL(triggered()),this,SLOT(ActionUpFileDialog()));

	fileMenu = new QMenu(this);	
	fileMenu->addAction(downloadfile);
	connect(downloadfile,SIGNAL(triggered()),this,SLOT(ActionDownFileDialog()));
}

void  FileWidget::MenuRequested(const QPoint &pos)
{
	QModelIndex index = treeview->indexAt(pos);
	QVariant    var = index.data(ROLE_MARK);
	if(var.isValid())
	{
		if(var.toInt() == MARK_FOLDER)
			folderMenu->exec(QCursor::pos());
		else if(var.toInt() == MARK_FILE)
			fileMenu->exec(QCursor::pos());
	}
}

void FileWidget::ActionUpFileDialog()
{
	filedialog = new QFileDialog;
	filedialog->setDirectory(".");
	filedialog->setFilter("*");
    filedialog->show();

	if(filedialog->exec() == QFileDialog::Accepted)
	{
		QString filename = filedialog->selectedFiles()[0];
	
		fileinfo = QFileInfo(filename);

		AddFile(fileinfo);

		QString file_name;
		file_name = fileinfo.fileName();
		
		char*   ch = new char[30];
		QByteArray ba = filename.toLocal8Bit();
		ch = ba.data();
		cout<<ch<<endl;
	
	//	string str = filename.toStdString();
		string str = ba.data();
		cout<<str<<endl;
		ba = file_name.toLocal8Bit();
		str = ba.data();
		cout<<str<<endl;
	}
}

void FileWidget::ActionDownFileDialog()
{
	QDialog *DownDialog = new QDialog(this);
	filedialog = new QFileDialog;
	filedialog->setFileMode(QFileDialog::DirectoryOnly);//只显示目录
    savelabel = new QLabel(tr("路径："));
	saveline = new QLineEdit;
	savebutton = new QPushButton(tr("保存"));
	connect(filedialog,SIGNAL(currentChanged(const QString &)),this,SLOT(LinePathShow(const QString &)));

	QHBoxLayout *saveLayout = new QHBoxLayout;
	saveLayout->addWidget(savelabel);
	saveLayout->addWidget(saveline);
	saveLayout->addWidget(savebutton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(filedialog);
	mainLayout->addLayout(saveLayout);
	DownDialog->setLayout(mainLayout);

	DownDialog->show();
}

void FileWidget::LinePathShow(const QString &path)
{
	saveline->setText(path);
}

void FileWidget::AddFile(QFileInfo  fileinfo)
{
	QString  file_suffix,file_name;
	file_suffix = fileinfo.suffix();
	file_name = fileinfo.fileName();
    QStandardItem*   file = new QStandardItem(file_name);
    file->setData(MARK_FILE,ROLE_MARK);

	if(operator==("doc",file_suffix))
	{
	    itemDocument->appendRow(file);
	}
	else if(operator==("png",file_suffix))
	{
		itemPicture->appendRow(file);
	}
	else if(operator==("mp4",file_suffix))
	{
		itemVideo->appendRow(file);
	}
	else if(operator==("mp3",file_suffix))
	{
		itemmusic->appendRow(file);
	}
	else
	{
		itemother->appendRow(file);
	}
}
