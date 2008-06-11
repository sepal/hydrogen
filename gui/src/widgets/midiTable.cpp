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

#include "ClickableLabel.h"

#include <QtGui>
#include "midiTable.h"




#include <QPixmap>
#include <QFontDialog>

#include "midiTable.h"

#include <hydrogen/midiMap.h>
#include <hydrogen/hydrogen.h>
#include <hydrogen/Preferences.h>
#include <hydrogen/IO/MidiInput.h>

#include <hydrogen/globals.h>

midiTable::midiTable( QWidget *pParent )
 : QTableWidget( pParent ),Object("midiTable")
{
	setupMidiTable();
}

midiTable::~midiTable()
{
	int myRow = 0;
	
	for( myRow = 0; myRow <  rowCount() ; myRow++)
	{
		delete cellWidget(myRow,0);
		delete cellWidget(myRow,1);
		delete cellWidget(myRow,2);
		delete cellWidget(myRow,3);
	}
}


void midiTable::setupMidiTable()
{
	actionManager *aH = actionManager::getInstance();
	midiMap *mM = midiMap::getInstance();

	QStringList items;
	items <<  "Event"  <<  "Param."  <<  "Action" 
           <<  "Param." ;


	int rowCount;
	
	rowCount = ( mM->getMMCMap() ).size();

	setRowCount( 0 );
    	setColumnCount( 4 );

	verticalHeader()->hide();

	setHorizontalHeaderLabels( items );
	
	
	setFixedWidth( 500 );
	setColumnWidth( 0 , 175 );
	setColumnWidth( 1, 73 );
	setColumnWidth( 2, 175 );
	setColumnWidth( 3 , 73 );


	rowCount= 0;	
	
	std::map< QString , action *> mmcMap = mM->getMMCMap();
	std::map< QString , action *>::iterator dIter(mmcMap.begin());


	for( dIter = mmcMap.begin(); dIter != mmcMap.end(); dIter++ )
	{
		insertRow( rowCount );
		QString eventString = dIter->first;

		QComboBox *eventBox = new QComboBox();
		eventBox->insertItems( rowCount , aH->getEventList() );
		eventBox->setCurrentIndex( eventBox->findText(eventString) );
		setCellWidget( rowCount, 0, eventBox );
		
		QSpinBox *eventParameterSpinner = new QSpinBox();
		setCellWidget( rowCount , 1, eventParameterSpinner );

		action * pAction = mM->getMMCAction( eventString );
	
		QComboBox *actionBox = new QComboBox();
		actionBox->insertItems(rowCount,aH->getActionList());
		actionBox->setCurrentIndex ( actionBox->findText(pAction->getType() ) );
		setCellWidget( rowCount , 2, actionBox );
	

		QString actionParameter;
		if( pAction->getParameterList().size() != 0 ){
			actionParameter = pAction->getParameterList().at(0);
		}

		QSpinBox *actionParameterSpinner = new QSpinBox();
		
		bool ok;
		actionParameterSpinner->setValue( actionParameter.toInt(&ok,10) );
		
		setCellWidget( rowCount , 3, actionParameterSpinner );
		rowCount++;
	}

	for( int note = 0; note < 128; note++ ){
		
		QString eventString = "NOTE";
		
		action * pAction = mM->getNoteAction( note );
		
		if( pAction->getType() != "NOTHING" ){
		
			insertRow( rowCount );
			QComboBox *eventBox = new QComboBox();
			eventBox->insertItems( rowCount , aH->getEventList() );
			eventBox->setCurrentIndex( eventBox->findText(eventString) );
			setCellWidget( rowCount, 0, eventBox );
			
			QSpinBox *eventParameterSpinner = new QSpinBox();
			setCellWidget( rowCount , 1, eventParameterSpinner );
			eventParameterSpinner->setValue( note );
			
			QComboBox *actionBox = new QComboBox();
			actionBox->insertItems(rowCount,aH->getActionList());
			actionBox->setCurrentIndex ( actionBox->findText(pAction->getType() ) );
			setCellWidget( rowCount , 2, actionBox );
		
	
			QString actionParameter;
			QSpinBox *actionParameterSpinner = new QSpinBox();

			if( pAction->getParameterList().size() > 0 ){
				actionParameter = pAction->getParameterList().at(0);
				bool ok;
				actionParameterSpinner->setValue( actionParameter.toInt(&ok,10) );
			}
	
			setCellWidget( rowCount , 3, actionParameterSpinner );
			rowCount++;
		}
	}
	
	insertRow( rowCount );

	QComboBox *eventBox = new QComboBox();
	eventBox->insertItems( rowCount , aH->getEventList() );
	setCellWidget( rowCount, 0, eventBox );
		
	QSpinBox *eventParameterSpinner = new QSpinBox();
	eventParameterSpinner->setEnabled( false );
	setCellWidget( rowCount , 1, eventParameterSpinner );

	QComboBox *actionBox = new QComboBox();
	actionBox->insertItems( rowCount, aH->getActionList() );
	setCellWidget( rowCount, 2, actionBox );
			
	QSpinBox *actionParameterSpinner = new QSpinBox();
	setCellWidget( rowCount, 3, actionParameterSpinner );
}


void midiTable::saveMidiTable(){
	
	delete	midiMap::getInstance();
	midiMap *mM  = midiMap::getInstance();
	

	int row = 0;
	
	for( row = 0; row <  rowCount(); row++ ){

		QComboBox * eventCombo =  dynamic_cast <QComboBox *> ( cellWidget( row, 0 ) );
	
		QSpinBox * eventSpinner = dynamic_cast <QSpinBox *> ( cellWidget( row, 1 ) );

		QComboBox * actionCombo = dynamic_cast <QComboBox *> ( cellWidget( row, 2 ) );

		QSpinBox * actionSpinner = dynamic_cast <QSpinBox *> ( cellWidget( row, 3 ) );

		

		QString eventString;
		QString actionString;
		

		if( eventCombo->currentText() != "" && actionCombo->currentText() != "" ){
			eventString = eventCombo->currentText();

			actionString = actionCombo->currentText();
		
			action * pAction = new action( actionString );

			if( actionSpinner->cleanText() != ""){
				pAction->addParameter( actionSpinner->cleanText() );
			}
	
			if( eventString.left(3) == "MMC" ){
				mM->registerMMCEvent( eventString , pAction );
			}
			
			if( eventString.left(4) == "NOTE" ){
				mM->registerNoteEvent( eventSpinner->cleanText().toInt() , pAction );
			}
		}
	}
}