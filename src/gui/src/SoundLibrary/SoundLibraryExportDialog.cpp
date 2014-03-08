/*
 * Hydrogen
 * Copyright(c) 2002-2008 by Alex >Comix< Cominu [comix@users.sourceforge.net]
 *
 * http://www.hydrogen-music.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "SoundLibraryExportDialog.h"

#include <hydrogen/hydrogen.h>
#include <hydrogen/helpers/filesystem.h>
#include <hydrogen/Preferences.h>
#include <hydrogen/h2_exception.h>

#include <hydrogen/basics/adsr.h>
#include <hydrogen/basics/sample.h>
#include <hydrogen/basics/instrument.h>
#include <QFileDialog>
#include <memory>
#include <QtGui>

using namespace H2Core;

const char* SoundLibraryExportDialog::__class_name = "SoundLibraryExportDialog";

SoundLibraryExportDialog::SoundLibraryExportDialog( QWidget* pParent,  const QString& selectedKit )
	: QDialog( pParent )
	, Object( __class_name )
{
	setupUi( this );
	INFOLOG( "INIT" );
	setWindowTitle( trUtf8( "Export Sound Library" ) );
	setFixedSize( width(), height() );
	preselectedKit = selectedKit;
	updateDrumkitList();
	drumkitPathTxt->setText( QDir::homePath() );
}




SoundLibraryExportDialog::~SoundLibraryExportDialog()
{
	INFOLOG( "DESTROY" );

	for (uint i = 0; i < drumkitInfoList.size(); i++ ) {
		Drumkit* info = drumkitInfoList[i];
		delete info;
	}
	drumkitInfoList.clear();
}



void SoundLibraryExportDialog::on_exportBtn_clicked()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);

	QString drumkitName = drumkitList->currentText();
	QString drumkitDir = Filesystem::drumkit_dir_search( drumkitName );
	QString saveDir = drumkitPathTxt->text();
	QString cmd = QString( "cd " ) + drumkitDir + "; tar czf \"" + saveDir + "/" + drumkitName + ".h2drumkit\" -- \"" + drumkitName + "\"";
	int ret = system( cmd.toLocal8Bit() );

	QApplication::restoreOverrideCursor();
	QMessageBox::information( this, "Hydrogen", "Drumkit exported." );
}

void SoundLibraryExportDialog::on_drumkitPathTxt_textChanged( QString str )
{
	QString path = drumkitPathTxt->text();
	if (path.isEmpty()) {
		exportBtn->setEnabled( false );
	}
	else {
		exportBtn->setEnabled( true );
	}
}

void SoundLibraryExportDialog::on_browseBtn_clicked()
{
	static QString lastUsedDir = QDir::homePath();
	QString filename = QFileDialog::getExistingDirectory (this, tr("Directory"), lastUsedDir);
	if ( filename.isEmpty() ) {
		drumkitPathTxt->setText( QDir::homePath() );
	}
	else
	{
		drumkitPathTxt->setText( filename );
		lastUsedDir = filename;
	}
}

void SoundLibraryExportDialog::updateDrumkitList()
{
	INFOLOG( "[updateDrumkitList]" );

	drumkitList->clear();

	for (uint i = 0; i < drumkitInfoList.size(); i++ ) {
		Drumkit* info = drumkitInfoList[i];
		delete info;
	}
	drumkitInfoList.clear();

	QStringList sysDrumkits = Filesystem::sys_drumkits_list();
	for (int i = 0; i < sysDrumkits.size(); ++i) {
		QString absPath = Filesystem::sys_drumkits_dir() + "/" + sysDrumkits.at(i);
		Drumkit *info = Drumkit::load( absPath );
		if (info) {
			drumkitInfoList.push_back( info );
			drumkitList->addItem( info->get_name() );
		}
	}

	QStringList userDrumkits = Filesystem::usr_drumkits_list();
	for (int i = 0; i < userDrumkits.size(); ++i) {
		QString absPath = Filesystem::usr_drumkits_dir() + "/" + userDrumkits.at(i);
		Drumkit *info = Drumkit::load( absPath );
		if (info) {
			drumkitInfoList.push_back( info );
			drumkitList->addItem( info->get_name() );
		}
	}

	/*
	 * If the export dialog was called from the soundlibrary panel via right click on
	 * a soundlibrary, the variable preselectedKit holds the name of the selected drumkit
	 */

	int index = drumkitList->findText( preselectedKit );
	if ( index >= 0)
		drumkitList->setCurrentIndex( index );
	else
		drumkitList->setCurrentIndex( 0 );
}
