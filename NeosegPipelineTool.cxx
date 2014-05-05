#include "NeosegPipelineTool.h"

/*!
 * \file NeosegPipelineTool.cxx
 * \brief Main class of NeosegPipeline, source file
 * \author Marie Cherel
 */


/*!
*  \brief Constructor
*  creates 
*
*/
NeosegPipelineTool::NeosegPipelineTool()
{
   m_parameters = new::PipelineParameters();

   m_pipeline = new::Pipeline();
   m_pipeline->setPipelineParameters(m_parameters); 

   m_parametersErrors = ""; 
   m_executablesErrors = "";
}

void NeosegPipelineTool::setProgramPath(QString programPath)
{
   m_parameters->setProgramPath(programPath);
}
void NeosegPipelineTool::setOutput(QString output)
{
      m_parameters->setOutput(output);
}
void NeosegPipelineTool::setT1(QString T1)
{
   if(m_parameters->checkT1(T1))
   {
      m_parameters->setT1(T1);
   }
}
void NeosegPipelineTool::setT2(QString T2)
{
   if(m_parameters->checkT2(T2))
   {
      m_parameters->setT2(T2);
   }
}
void NeosegPipelineTool::setMask(QString mask)
{
   if(m_parameters->checkMask(mask))
   {
      m_parameters->setMask(mask);
   }
}
void NeosegPipelineTool::setDWI(QString DWI)
{
   if(m_parameters->checkDWI(DWI))
   {
      m_parameters->setDWI(DWI);
   }
}
void NeosegPipelineTool::setb0(QString b0)
{
   if(m_parameters->checkb0(b0))
   {
      m_parameters->setb0(b0);
   }
}
void NeosegPipelineTool::setParametersFile(QString xmlFile) 
{
   if(!xmlFile.isEmpty())
   {
      XmlReader m_xmlReader;
      m_xmlReader.setPipelineParameters(m_parameters);
      m_parametersErrors = m_xmlReader.readParametersConfigurationFile(xmlFile); 
   }
}

void NeosegPipelineTool::setExecutablesFile(QString xml_CLI) 
{
   XmlReader m_xmlReader;
   m_xmlReader.setPipelineParameters(m_parameters);

   if(!xml_CLI.isEmpty())
   {
      m_xmlReader.readExecutablesConfigurationFile(xml_CLI); 
   }
   else
   {
      QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
      QString xml_ENV = env.value("NEOSEG_PIPELINE_EXECUTABLES", QString::null);

      if(!xml_ENV.isEmpty())
      {
         m_executablesErrors = m_xmlReader.readExecutablesConfigurationFile(xml_ENV);
      }
   }
}

void NeosegPipelineTool::printErrors()
{
   if(!m_parametersErrors.isEmpty() || !m_executablesErrors.isEmpty())
   {
      if(!m_parametersErrors.isEmpty() && !m_executablesErrors.isEmpty())
      {
         m_errors = "Errors in the parameters configuration :\n" + m_parametersErrors;
         m_errors += "\n";
         m_errors += "Errors in the software configuration :\n" + m_executablesErrors;           
      }

      if(!m_parametersErrors.isEmpty() && m_executablesErrors.isEmpty())      
      {
         m_errors = "Errors in the parameters configuration :\n" + m_parametersErrors; 
      }

      if(m_parametersErrors.isEmpty() && !m_executablesErrors.isEmpty())      
      {
         m_errors = "Errors in the software configuration :\n" + m_executablesErrors; 
      }

      m_errors += "\n";
      m_errors += "All the parameters or softwares that are nor valid, are left to their default value\n";
   }
}

void NeosegPipelineTool::launch(int argc, char *argv[], bool gui)
{
   printErrors();

   if(gui)
   {
      QApplication app(argc, argv);
      MainScriptThread* thread = new::MainScriptThread(); 

      DerivedWindow window;
      window.setPipelineParameters(m_parameters); 
      window.setPipeline(m_pipeline);
      window.setMainScriptThread(thread);
      window.show();

      if(!m_errors.isEmpty())
      {
         window.printErrors(m_errors);  
      }

      app.exec();
      thread->wait();
   }
   else
   {
      QString output = m_parameters->getOutput();    

      if(!(QFileInfo(output).exists()))
      { 
         std::cout<<output.toStdString()<<" does not exist, do you want to create it? (y/n) : "; 

         std::string answer;
         std::cin>>answer;

         if(answer == "y")
         {
            bool result = (QDir::root()).mkpath(output);
            if(!result)
            {
               std::cout<<output.toStdString()<<" can not be created, Please try again with a new directory path"<<std::endl; 
               return; 
            }            
         }
         else
         {
            return; 
         }

      }

      if(!m_errors.isEmpty())
      {
         std::cout<<m_errors.toStdString()<<std::endl; 
         std::cout<<"Do you want to run the pipeline anyway? (y/n) : "; 

         std::string answer;
         std::cin>>answer;

         if(answer != "y")
         {
            return;
         }
      }

      m_pipeline->writePipeline(); 
      m_pipeline->runPipeline();

      return; 
   }   
}
	 
