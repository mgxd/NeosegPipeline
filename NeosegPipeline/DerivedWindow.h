#ifndef DEF_DerivedWindow
#define DEF_DerivedWindow

// General Librairies
#include <iostream>
#include <string>
#include <cstring>
#include <map>

// Qt Librairies
#include <QWidget>
#include <QtGui>  //MessageBox
#include <QMap>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QList>

// My Specific Librairies
#include "ui_Window.h"
#include "ui_About.h"
#include "ui_neosegParameters.h"
#include "ui_ABCParameters.h"
#include "Pipeline.h"
#include "XmlReader.h"
#include "XmlWriter.h"
#include "MainScriptThread.h"
#include "ExecutablePaths.h" 
#include "LibraryPaths.h"
#include "About.h" 


#ifndef NEOSEGPIPELINE_VERSION
#define NEOSEGPIPELINE_VERSION "unknown"
#endif

class DerivedWindow : public QMainWindow , public Ui_Window
{
   Q_OBJECT // Enable slots and signals

   struct Executable
   {
      QPushButton* select_button;
      QLineEdit* enter_lineEdit; 
      QPushButton* reset_button; 
   };

   struct Library
   {
      QPushButton* select_button;
      QLineEdit* enter_lineEdit; 
   };

   struct Image
   {
      QPushButton* select_button;
      QLineEdit* enter_lineEdit; 
   }; 

   public:

   // Constructor
   DerivedWindow();

   // Drag and drop
   void dragEnterEvent(QDragEnterEvent *event);
   void dropEvent(QDropEvent* event);

   // Set
   void setPipeline(Pipeline* pipeline);
   void setPipelineParameters(PipelineParameters* parameters);
   void setMainScriptThread(MainScriptThread* thread);

   // Print XML Errors
   void printErrors(QString errors);

   // Initialization
   void initializeImagesMap();
   void initializeExecutablesMap();
   void initializeLibrariesMap();
   void initializeXMLParameters();
   void initializeExecutables();
   void initializeDataParameters();

   // Connection 

   // Atlas Population
   void checkAtlases();
   void displayAtlases();
   void checkSelectedAtlases();

   // Parameter
   void setData();
   void setParameters(); 
   void setExecutables();


   public slots :

   // About 
   void about(); 

   // Output
   void selectOuput();
   void enterOutput();
   void createOutput(QString output);

   // Prefix 
   void enterPrefix(); 

   // Suffix
   void enterSuffix();

   // Images
   void selectImage(QString image); 
   void enterImage(QString image);

   // New or Existing Atlas
   void selectNewOrExistingAtlas();

   // New Atlas
   void selectAtlasPopulationDirectory();
   void enterAtlasPopulationDirectory();
   void resetAtlasPopulationDirectory();
   void UpdateAtlasPopulationDirectoryDisplay();
   void selectAtlas(QListWidgetItem* item);

   // Existing Atlas
   void selectExistingAtlas();
   void enterExistingAtlas();

   // Preprocessing Data //
   void changeUsingMaskDTI(bool) ;

   // Atlas Registration //
   void changeUsingMaskAtlas(bool) ;

   //Tissue Segmentation
   void tissueSegmentationSoftwareSelection() ;

   // Computing System
   void changeComputingSystem(int index);

   // Executables/Data/Parameters
   void selectXMLFile(int XMLFileType ) ;
   void loadXMLFile(QString xmlFileName , int XMLFileType );

   // Parameters
   void saveParameters();  
   void saveExecutables();
   void saveData();

   // Executables 
   void selectExecutable(QString executable_name);
   void enterExecutable(QString executable_name);
   void resetExecutable(QString executable_name);
   void resetAllExecutables(); 

   // Executables 
   void selectLibrary(QString library_name);
   void enterLibrary(QString library_name);

   // Run Pipeline
   void runPipeline();

   // Logging
   void initializePipelineLogging();
   void initializeRegistrationsLogging();

   void printPipelineLog();    
   void printRegistrationLog(QString file_name); 

   void selectLog();
   void changeExecutionPlainTextEdit();

   // Enable widgets 
   void setParametersWidgetEnabled(bool enabled);
   
   // Stop Pipeline
   void stopPipeline(); 

   // Display Results
   void enableDisplayButton();
   void displayResults(); 

   // Exit Application 
   void closeEvent(QCloseEvent *event);

   //ABC dynamic UI
   void updateNumbersOfPriorsForABC(int nbPriors) ;

private slots:
   void on_comboBoxOutputImageFormat_currentIndexChanged(const QString &arg1);

private :
   
   // Window
   Ui_Window ui;
   Ui::ABCParameters *abcParameters ;
   Ui::neosegParameters *neosegParameters ;
   // Pipeline 
   Pipeline* m_pipeline;

   // Parameters
   PipelineParameters* m_parameters;
   AntsParameters* m_antsParameters_DTI; 
   AntsParameters* m_antsParameters_atlas; 
   NeosegParameters* m_neosegParameters; 

   // Executables 
   ExecutablePaths* m_executables; 

   // Libraries
   LibraryPaths* m_libraries;

   QString m_tests_path;
   QString m_data_path;
   QString m_existingAtlases_path;
   QStringList m_goodAtlases;
   QStringList m_wrongAtlases;  
   QStringList m_selectedAtlases;

   //ABC dynamic UI
   class PriorSpinBox : public QHBoxLayout{
   public:
       PriorSpinBox(int n){

           labelsp = new QLabel();
           labelsp->setText( QString("Prior %1 coefficient:").arg(n) ) ;
           this->addWidget(labelsp );


           dspin = new QDoubleSpinBox();
           dspin->setSingleStep(0.1);
           dspin->setMinimum(0);
           dspin->setValue(1);
           this->addWidget(dspin);
       }

       ~PriorSpinBox(){
           delete labelsp;
           delete dspin;
       }
       QLabel *labelsp;
       QDoubleSpinBox *dspin;
   };
   std::vector<PriorSpinBox*> vectorABCPriorCheckBoxes ;

   bool m_parametersSet;
   bool m_executablesSet;
   bool m_pipelineWriten; 
   QString m_abcOutputImageFormat;

   MainScriptThread* m_thread; 

   QMap<QString, Image> m_images_map; 
   QMap<QString, Executable> m_executables_map; 
   QMap<QString, Library> m_libraries_map; 

   struct Logging
   {
      QTextStream* textStream;
      QPlainTextEdit* plainTextEdit; 
   };

   QTextStream* m_log_textStream; 

   QMap<QString, Logging> m_registrationLoggings; 

   About* m_about; 
   

};

#endif
