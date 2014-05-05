#include "PipelineParameters.h"

PipelineParameters::PipelineParameters()
{  
   m_forbiddenCharacters << "\\" << "/" << ":" << "*" << "?" << "\"" << "<" << ">" << "|" << " ";
   m_prefix = "neo";
   m_suffix = "NP";

   m_newAtlas_default = true; 
   m_newAtlas = m_newAtlas_default;

   QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
   m_atlasPopulationDirectory_default = env.value("NEOSEG_PIPELINE_ATLAS_POPULATION_DIRECTORY", QString::null);   
   m_atlasPopulationDirectory = m_atlasPopulationDirectory_default; 

   m_precision = 10000;
   QStringList atlases = QDir(m_atlasPopulationDirectory).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
   QStringList::iterator it; 
   for(it = atlases.begin(); it != atlases.end(); it++ )
   {
      if(checkAtlas(*it))
      {
         m_selectedAtlases << *it;
      }
   }


   m_smoothing_values << "gaussian" << "curve evolution";
   m_smoothing_default = m_smoothing_values[0];
   m_smoothing = m_smoothing_default;

   m_smoothingSize_min = 0;
   m_smoothingSize_max = 6;
   m_smoothingSize_default = 1;
   m_smoothingSize = m_smoothingSize_default;

   m_computingWeights_default=1;
   m_computingWeights=m_computingWeights_default;

   m_weightsModality_values << "T1" << "T2";
   m_weightsModality_default = m_weightsModality_values[0];
   m_weightsModality=m_weightsModality_default;

   m_weightsRadius_min=0;
   m_weightsRadius_max=6;
   m_weightsRadius_default=3;
   m_weightsRadius=m_weightsRadius_default;

   m_weightsRadiusUnit_values << "mm" << "pixels";
   m_weightsRadiusUnit_default = m_weightsRadiusUnit_values[0];
   m_weightsRadiusUnit = m_weightsRadiusUnit_default;

   // Including FA 
   m_includingFA_default = true; 
   m_includingFA = m_includingFA_default; 

   // FA Weight
   m_FAWeight_min = 0;
   m_FAWeight_default = 1.5; 
   m_FAWeight = m_FAWeight_default; 

   // FA Smoothing Size 
   m_FASmoothingSize_min = 0; 
   m_FASmoothingSize_default = 1; 
   m_FASmoothingSize = m_FASmoothingSize_default; 
   
   // Usign FA
   m_usingFA_default = false; 
   m_usingFA = m_usingFA_default;

   // Using AD
   m_usingAD_default = true; 
   m_usingAD = m_usingAD_default;

   // Computing 3-labels Seg 
   m_computing3LabelsSeg_default = true; 
   m_computing3LabelsSeg = m_computing3LabelsSeg_default;

   // Reassigning White Matter
   m_reassigningWhiteMatter_default = true; 
   m_reassigningWhiteMatter = m_reassigningWhiteMatter_default;

   // Size Threshold 
   m_sizeThreshold_min = 0; 
   m_sizeThreshold_default = 200; 
   m_sizeThreshold = m_sizeThreshold_default; 

   // Overwritting
   m_overwriting_default = 0;
   m_overwriting = m_overwriting_default;

   // Clening-Up
   m_cleaningUp_default = 1;
   m_cleaningUp = m_cleaningUp_default;

   // Stopping If Error 
   m_stoppingIfError_default = true; 
   m_stoppingIfError = m_stoppingIfError_default; 

   // Computing System
   m_computingSystem_values << "local" << "killdevil";
   m_computingSystem_default = m_computingSystem_values[0];
   m_computingSystem = m_computingSystem_default;

   m_numberOfCores_min = 0;
   m_numberOfCores_max = 20;
   m_numberOfCores_default = 1;
   m_numberOfCores = m_numberOfCores_default;

   m_antsParameters = new AntsParameters(); 

   m_neosegParameters = new NeosegParameters(); 

   m_executablePaths = new ExecutablePaths(); 

   m_executablePaths->setDefaultExecutablePath("ImageStat");
   m_executablePaths->setDefaultExecutablePath("SegPostProcessCLP");
   m_executablePaths->setDefaultExecutablePath("N4ITKBiasFieldCorrection");
   m_executablePaths->setDefaultExecutablePath("ANTS");
   m_executablePaths->setDefaultExecutablePath("ResampleVolume2");
   m_executablePaths->setDefaultExecutablePath("ImageMath");
   m_executablePaths->setDefaultExecutablePath("ITKTransformTools");
   m_executablePaths->setDefaultExecutablePath("dtiestim");
   m_executablePaths->setDefaultExecutablePath("dtiprocess");
   m_executablePaths->setDefaultExecutablePath("bet2");
   m_executablePaths->setDefaultExecutablePath("unu");
   m_executablePaths->setDefaultExecutablePath("InsightSNAP");
   m_executablePaths->setDefaultExecutablePath("HistogramMatching");
   m_executablePaths->setExecutablePath("SpreadFA", "/work/mcherel/project/neosegPipeline/spreadFA/bin/SpreadFA");
   m_executablePaths->setExecutablePath("neoseg", "/work/mcherel/project/neoseg/bin/bin_lessRefinement/neoseg");
   m_executablePaths->setExecutablePath("ReassignWhiteMatter", "/work/mcherel/project/neosegPipeline/reassignWhiteMatter/bin/ReassignWhiteMatter");
}

// Checking Functions

bool PipelineParameters::isSuperior(int value, int min)
{
   if(value>min)
   {
      return true; 
   }
   return false; 
}

bool PipelineParameters::isSuperior(double value, double min)
{
   if(value>min)
   {
      return true; 
   }
   return false; 
}

bool PipelineParameters::isBetween(int value, int min, int max)
{
   if(value>min && value<max)
   {
      return true; 
   }
   return false; 
}

bool PipelineParameters::isBetween(double value, double min, double max)
{
   if(value>min && value<max)
   {
      return true; 
   }
   return false; 
}


bool PipelineParameters::isIn(QString item, QStringList list)
{
   QStringList::iterator it; 
   for(it=list.begin(); it!=list.end(); ++it)
   {
      if(item.compare(*it, Qt::CaseInsensitive)==0)
      {
         return true;
      }
   }
   return false; 
}

QFileInfoList PipelineParameters::find(QDir* dir, QString name)
{
   name = "*" + name + "*";
   QStringList* filter = new QStringList(name);
   QFileInfoList foundFile_list = dir->entryInfoList(*filter);

   return foundFile_list;
} 


bool PipelineParameters::checkPrefixSuffix(QString string)
{
   QStringList::iterator it; 
   for(it = m_forbiddenCharacters.begin(); it != m_forbiddenCharacters.end(); ++it)
   {
      if(string.contains(*it))
      {
         return false; 
      }
   }
   return true; 
}

// Program Path
void PipelineParameters::setProgramPath(QString programPath)
{
   m_programPath = programPath; 
   m_executablePaths->setProgramPath(programPath); 
}
QString PipelineParameters::getProgramPath()
{
   return m_programPath;  
}

// Prefix
void PipelineParameters::setPrefix(QString prefix) 
{
   m_prefix = prefix;
}
QString PipelineParameters::getPrefix() 
{
   return m_prefix;
}

// Suffix
void PipelineParameters::setSuffix(QString suffix) 
{
   m_suffix = suffix;
}
QString PipelineParameters::getSuffix() 
{
   return m_suffix;
}

QString PipelineParameters::getForbiddenCharacters()
{
   return m_forbiddenCharacters.join(" ");
}

// Output
bool PipelineParameters::checkOutput(QString output)
{
   return (QDir(output)).exists();
}
void PipelineParameters::setOutput(QString output) 
{
   m_output = QFileInfo(output).absoluteFilePath();
}
QString PipelineParameters::getOutput() 
{
   return m_output;
} 

// T1
bool PipelineParameters::checkT1(QString T1) 
{
   return (QFileInfo(T1)).isFile();
}
void PipelineParameters::setT1(QString T1)
{
   m_T1 = QFileInfo(T1).absoluteFilePath();
}
QString PipelineParameters::getT1()
{
   return m_T1;
} 

// T2 
bool PipelineParameters::checkT2(QString T2)
{
   return (QFile(T2)).exists();
}
void PipelineParameters::setT2(QString T2)
{
   m_T2 = QFileInfo(T2).absoluteFilePath();
}
QString PipelineParameters::getT2()
{
   return m_T2;
}

// Mask
bool PipelineParameters::checkMask(QString mask)
{
   return (QFile(mask)).exists();
}
void PipelineParameters::setMask(QString mask)
{
   m_mask = QFileInfo(mask).absoluteFilePath();
}
QString PipelineParameters::getMask()
{
   return m_mask;
}

// DWI
bool PipelineParameters::checkDWI(QString DWI) 
{
   return (QFile(DWI)).exists();
}
void PipelineParameters::setDWI(QString DWI)
{
   m_DWI = QFileInfo(DWI).absoluteFilePath();
}
QString PipelineParameters::getDWI()
{
   return m_DWI;
}

// b0
bool PipelineParameters::checkb0(QString b0)
{
   return (QFile(b0)).exists();
}
void PipelineParameters::setb0(QString b0)
{
   m_b0 = QFileInfo(b0).absoluteFilePath();
}
QString PipelineParameters::getb0()
{
   return m_b0;
}

// Neo
void PipelineParameters::initializeNeo()
{
   m_neo.prefix = m_prefix;
   m_neo.T1 = m_T1; 
   m_neo.T2 = m_T2;
   m_neo.mask = m_mask; 
   m_neo.DWI = m_DWI; 
   m_neo.b0 = m_b0; 
}
void PipelineParameters::setNeo(Neo neo)
{
   m_neo=neo;
}
Neo PipelineParameters::getNeo()
{
   return m_neo;
}


// New Atlas
void PipelineParameters::setNewAtlas(bool newAtlas)
{
   m_newAtlas=newAtlas;
}
bool PipelineParameters::getNewAtlas()
{
   return m_newAtlas;
}

// Atlas
bool PipelineParameters::checkExistingAtlas(QString atlas)
{
   QDir* atlas_dir = new QDir(atlas);

   if(atlas_dir->exists())
   {
      QFileInfoList templateT1 = find(atlas_dir, "templateT1");
      QFileInfoList templateT2 = find(atlas_dir, "templateT2");

      QFileInfoList white = find(atlas_dir, "white");
      QFileInfoList gray = find(atlas_dir, "gray");
      QFileInfoList csf = find(atlas_dir, "csf");
      QFileInfoList rest = find(atlas_dir, "rest");

      if (templateT1.size()==1 && templateT2.size()==1 && white.size()==1 && gray.size()==1 && csf.size()==1 && rest.size()) 
      {
         return true;
      }
   }

   return false;
}
void PipelineParameters::setAtlas(QString atlas)
{
   m_atlas=atlas;
}
QString PipelineParameters::getAtlas()
{
   return m_atlas;
}

// Atlas Population Directory
bool PipelineParameters::checkAtlasPopulationDirectory(QString atlasPopulation_path)
{
   return (QDir(atlasPopulation_path)).exists();
}
void PipelineParameters::setAtlasPopulationDirectory(QString atlasPopulationDirectory)
{
   m_atlasPopulationDirectory=atlasPopulationDirectory;
}
QString PipelineParameters::getAtlasPopulationDirectory() 
{
   return m_atlasPopulationDirectory;
}

// Selected Atlases
void PipelineParameters::setSelectedAtlases(QStringList selectedAtlases) 
{
   m_selectedAtlases = selectedAtlases;
}
QStringList PipelineParameters::getSelectedAtlases() 
{
   return m_selectedAtlases;
}

// Atlas Population 
QMap<QString,QFileInfoList> PipelineParameters::findAtlasFiles(QString atlas_name)
{
   QMap<QString,QFileInfoList> atlasFiles;
 
   QDir* atlasPopulation_dir = new QDir(m_atlasPopulationDirectory);
   QString atlas_path = atlasPopulation_dir->filePath(atlas_name);

   QDir* atlas_dir = new QDir(atlas_path);

   atlasFiles.insert("T1",find(atlas_dir, "T1"));
   atlasFiles.insert("T2",find(atlas_dir, "T2"));
   atlasFiles.insert("seg",find(atlas_dir, "seg"));
   atlasFiles.insert("white",find(atlas_dir, "white"));
   atlasFiles.insert("gray",find(atlas_dir, "gray"));
   atlasFiles.insert("csf",find(atlas_dir, "csf"));

   return atlasFiles;
}

bool PipelineParameters::checkAtlasFiles(QString atlas)
{
   QMap<QString,QFileInfoList> atlasFiles = findAtlasFiles(atlas);  

   if ((atlasFiles["T1"]).size() == 1 || (atlasFiles["T2"]).size() == 1)
   {
      // Hard segmentation
      if ((atlasFiles["seg"]).size() == 1)
      {
         return true;
      }

      // Soft segmentation
      if ((atlasFiles["white"]).size() == 1 && (atlasFiles["gray"]).size() == 1 && (atlasFiles["csf"]).size() == 1)
      {  
         return true; 
      }
   }
   return false; 
}

bool PipelineParameters::checkAtlasRange(QString atlas)
{
   QMap<QString,QFileInfoList> atlasFiles = findAtlasFiles(atlas); 

   if ((atlasFiles["white"]).size() == 1 && (atlasFiles["gray"]).size() == 1 && (atlasFiles["csf"]).size() == 1)
   {
      QString white_path = ((atlasFiles["white"])[0]).filePath(); 
      QString gray_path = ((atlasFiles["gray"])[0]).filePath(); 
      QString csf_path = ((atlasFiles["csf"])[0]).filePath(); 

      MinMax<float> white_minMax(white_path.toStdString());  
      MinMax<float> gray_minMax(gray_path.toStdString());  
      MinMax<float> csf_minMax(csf_path.toStdString());  

      float min_white = ((int)(white_minMax.GetMin())*m_precision)/m_precision; 
      float max_white = ((int)(white_minMax.GetMax())*m_precision)/m_precision; 

      float min_gray = ((int)(gray_minMax.GetMin())*m_precision)/m_precision; 
      float max_gray = ((int)(gray_minMax.GetMax())*m_precision)/m_precision; 

      float min_csf = ((int)(csf_minMax.GetMin())*m_precision)/m_precision; 
      float max_csf = ((int)(csf_minMax.GetMax())*m_precision)/m_precision;

      if( min_white < 0 || max_white > 1 || min_gray < 0 || max_gray > 1 || min_csf < 0 || max_csf > 1 )
      {
         return false; 
      }
   } 
   return true;
}


bool PipelineParameters::checkAtlas(QString atlas)
{ 
   QMap<QString,QFileInfoList> atlasFiles = findAtlasFiles(atlas);  

   if(checkAtlasFiles(atlas) && checkAtlasRange(atlas))
   {
      return true; 
   }
   return false; 
}

void PipelineParameters::initializeAtlasPopulation() 
{
   m_atlasPopulation.clear(); 

   QStringList::const_iterator it;

   for (it = m_selectedAtlases.constBegin(); it != m_selectedAtlases.constEnd(); ++it) 
   {
      Atlas atlas;

      QDir* atlasPopulation_dir = new QDir(m_atlasPopulationDirectory);
      QString atlas_path = atlasPopulation_dir->filePath(*it);

      atlas.name=*it;

      QMap<QString,QFileInfoList> atlasFiles = findAtlasFiles(atlas_path); 

      if((atlasFiles["T1"]).size() > 0)
      {
         atlas.T1 = (atlasFiles["T1"])[0].filePath();
      }
      else
      {
         atlas.T1 = ""; 
      }
 
      if((atlasFiles["T2"]).size() > 0)
      {
         atlas.T2 = (atlasFiles["T2"])[0].filePath();
      }
      else
      {
         atlas.T2 = ""; 
      }

      if( (atlasFiles["seg"]).size()==1 )
      {
         atlas.seg = (atlasFiles["seg"])[0].filePath();
         atlas.probabilistic = 0;
      }


      if( (atlasFiles["white"]).size()==1 && (atlasFiles["gray"]).size()==1 && (atlasFiles["csf"]).size()==1 )
      {
         atlas.white = (atlasFiles["white"])[0].filePath();
         atlas.gray = (atlasFiles["gray"])[0].filePath();
         atlas.csf = (atlasFiles["csf"])[0].filePath();
         atlas.probabilistic=1;
      }

      //atlas.log = QDir(atlas_path).filePath(atlas.name + ".log");

      m_atlasPopulation.push_back(atlas);
   } 
}
void PipelineParameters::setAtlasPopulation(std::vector<Atlas> atlasPopulation)
{
   m_atlasPopulation = atlasPopulation;
}
std::vector<Atlas> PipelineParameters::getAtlasPopulation()
{
   return m_atlasPopulation;
}

// Smoothing 
bool PipelineParameters::checkSmoothing(QString smoothing)
{
   return isIn(smoothing, m_smoothing_values);
}
QString PipelineParameters::getSmoothingFlag(QString regularizationType)
{
   if(regularizationType.compare("gauss", Qt::CaseInsensitive) || regularizationType.compare("gaussian", Qt::CaseInsensitive))
   {
      return "gauss";
   }

   if(regularizationType.compare("curveEvolution", Qt::CaseInsensitive) || regularizationType.compare("curve evolution", Qt::CaseInsensitive))
   {
      return "curveEvol";
   }
   
   return NULL;
}
void PipelineParameters::setSmoothing(QString smoothing)
{
   m_smoothing=getSmoothingFlag(smoothing);
}
QString PipelineParameters::getSmoothing()
{
   return m_smoothing;
} 
int PipelineParameters::getSmoothingIndex()
{
   return m_smoothing_values.indexOf(m_smoothing);
}
QStringList PipelineParameters::getSmoothingValues()
{
   return m_smoothing_values; 
}

// Smoothing Size
bool PipelineParameters::checkSmoothingSize(double smoothingSize)
{
   return isBetween(smoothingSize, m_smoothingSize_min, m_smoothingSize_max);
}
void PipelineParameters::setSmoothingSize(double smoothingSize) 
{
   m_smoothingSize=smoothingSize;
}
double PipelineParameters::getSmoothingSize()
{
   return m_smoothingSize;
}

// Computing Weights
void PipelineParameters::setComputingWeights(int computingWeights)
{
   m_computingWeights=computingWeights;
}
bool PipelineParameters::getComputingWeights()
{
   return m_computingWeights;
}

// Weights Modality 
bool PipelineParameters::checkWeightsModality(QString weightsModality)
{
   return isIn(weightsModality, m_weightsModality_values);
}
void PipelineParameters::setWeightsModality(QString weightsModality)
{
   m_weightsModality=weightsModality;
}
QString PipelineParameters::getWeightsModality()
{
   return m_weightsModality;
}
int PipelineParameters::getWeightsModalityIndex()
{
   return m_weightsModality_values.indexOf(m_weightsModality);
}
QStringList PipelineParameters::getWeightsModalityValues()
{
   return m_weightsModality_values;
}

// Weights Radius 
bool PipelineParameters::checkWeightsRadius(double weightsRadius)
{
   return isBetween(weightsRadius, m_weightsRadius_min, m_weightsRadius_max);
}
void PipelineParameters::setWeightsRadius(double weightsRadius)
{
   m_weightsRadius=weightsRadius;
}
double PipelineParameters::getWeightsRadius()
{
   return m_weightsRadius;
}

// Weights Radius Unit
bool PipelineParameters::checkWeightsRadiusUnit(QString weightsRadiusUnit)
{
   return isIn(weightsRadiusUnit, m_weightsRadiusUnit_values);
}
void PipelineParameters::setWeightsRadiusUnit(QString weightsRadiusUnit)
{
   m_weightsRadiusUnit = weightsRadiusUnit;
}
QString PipelineParameters::getWeightsRadiusUnit()
{
   return m_weightsRadiusUnit;
}
int PipelineParameters::getWeightsRadiusUnitIndex()
{
   return m_weightsRadiusUnit_values.indexOf(m_weightsRadiusUnit);
}
QStringList PipelineParameters::getWeightsRadiusUnitValues()
{
   return m_weightsRadiusUnit_values;
}

// Including FA
void PipelineParameters::setIncludingFA(bool includingFA)
{
   m_includingFA = includingFA;
}
bool PipelineParameters::getIncludingFA()
{
   return m_includingFA;
}

// FA Weight 
bool PipelineParameters::checkFAWeight(double FAWeight)
{
   return isSuperior(FAWeight, m_FAWeight_min);
}
void PipelineParameters::setFAWeight(double FAWeight)
{
   m_FAWeight = FAWeight; 
}
double PipelineParameters::getFAWeight()
{
   return m_FAWeight; 
}

// FA Smoothing Size
bool PipelineParameters::checkFASmoothingSize(double FASmoothingSize)
{
   return isSuperior(FASmoothingSize, m_FASmoothingSize_min);
}
void PipelineParameters::setFASmoothingSize(double FASmoothingSize)
{
   m_FASmoothingSize = FASmoothingSize; 
}
double PipelineParameters::getFASmoothingSize()
{
   return m_FASmoothingSize; 
}

// Using FA
void PipelineParameters::setUsingFA(bool usingFA)
{
   m_usingFA = usingFA; 
}
bool PipelineParameters::getUsingFA()
{
   return m_usingFA; 
}

// Using AD
void PipelineParameters::setUsingAD(bool usingAD)
{
   m_usingAD = usingAD; 
}
bool PipelineParameters::getUsingAD()
{
   return m_usingAD; 
}

// Computing 3-Labels Segmentation
void PipelineParameters::setComputing3LabelsSeg(bool computing3LabelsSeg)
{
   m_computing3LabelsSeg = computing3LabelsSeg;
}
bool PipelineParameters::getComputing3LabelsSeg()
{
   return m_computing3LabelsSeg;
} 

// Reassigning White Matter 
void PipelineParameters::setReassigningWhiteMatter(bool reassigningWhiteMatter)
{
   m_reassigningWhiteMatter = reassigningWhiteMatter; 
}
bool PipelineParameters::getReassigningWhiteMatter()
{
   return m_reassigningWhiteMatter; 
}

// White Threshold 
bool PipelineParameters::checkSizeThreshold(int sizeThreshold)
{
   return isSuperior(sizeThreshold, m_sizeThreshold_min);
}
void PipelineParameters::setSizeThreshold(int sizeThreshold)
{
   m_sizeThreshold = sizeThreshold; 
}
int PipelineParameters::getSizeThreshold()
{
   return m_sizeThreshold; 
}

// Overwriting
void PipelineParameters::setOverwriting(bool overwriting)
{
   m_overwriting=overwriting;
} 
bool PipelineParameters::getOverwriting()
{
   return m_overwriting;
}

// Cleaning-Up
void PipelineParameters::setCleaningUp(bool cleaningUp)
{
   m_cleaningUp=cleaningUp;
}
bool PipelineParameters::getCleaningUp()
{
   return m_cleaningUp;
}

// Stopping If Error
void PipelineParameters::setStoppingIfError(bool stoppingIfError)
{
   m_stoppingIfError = stoppingIfError; 
}
bool PipelineParameters::getStoppingIfError()
{
   return m_stoppingIfError; 
}

// Computing System
bool PipelineParameters::checkComputingSystem(QString computingSystem)
{
   return isIn(computingSystem, m_computingSystem_values);
}
void PipelineParameters::setComputingSystem(QString computingSystem)
{
   m_computingSystem=computingSystem;
}
QString PipelineParameters::getComputingSystem()
{
   return m_computingSystem;
}
int PipelineParameters::getComputingSystemIndex()
{
   return m_computingSystem_values.indexOf(m_computingSystem); 
}
QStringList PipelineParameters::getComputingSystemValues()
{
   return m_computingSystem_values; 
}

// Number Of Cores 
bool PipelineParameters::checkNumberOfCores(int numberOfCores)
{
   return isBetween(numberOfCores, m_numberOfCores_min, m_numberOfCores_max);
}
void PipelineParameters::setNumberOfCores(int numberOfCores)
{
   m_numberOfCores=numberOfCores;
}
int PipelineParameters::getNumberOfCores()
{
   return m_numberOfCores;
}

// ANTS Parameters
AntsParameters* PipelineParameters::getAntsParameters()
{
   return m_antsParameters;
}

// Neoseg Parameters
NeosegParameters* PipelineParameters::getNeosegParameters()
{
   return m_neosegParameters;
}

// Executable Paths 
ExecutablePaths* PipelineParameters::getExecutablePaths()
{
   return m_executablePaths;
}

// Segmentation
QString PipelineParameters::getSegmentation()
{
   if(m_computing3LabelsSeg)
   {
      return m_neo.seg3Labels;
   }
   else
   {
      return m_neo.seg4Labels;
   }
}

QString PipelineParameters::checkImages()
{
   QString errors; 

   if(m_T1.isEmpty())
   {
      errors += "You cannot run the pipeline without a T1\n";
   }

   if(m_T2.isEmpty())
   {
      errors += "You cannot run the pipeline without a T2\n";
   }

   if(m_mask.isEmpty())
   {
      errors += "You cannot run the pipeline without a mask\n"; 
   }

   if(m_DWI.isEmpty())
   {
      if(m_includingFA)
      {
         errors += "You cannot include the FA in the probability maps without a DWI\n"; 
      }
      if(m_usingFA)
      {
         errors += "You cannot use the FA in Neoseg without a DWI\n"; 
      }
      if(m_usingAD)
      {
         errors += "You cannot use the AD in Neoseg without a DWI\n"; 
      }      
   }

   if(m_b0.isEmpty())
   {
      if(m_includingFA)
      {
         errors += "You cannot include the FA in the probability maps without a b0\n"; 
      }
      if(m_usingFA)
      {
         errors += "You cannot use the FA in Neoseg without a b0\n"; 
      }
      if(m_usingAD)
      {
         errors += "You cannot use the AD in Neoseg without a b0\n"; 
      }      
   }
   return errors;
}


