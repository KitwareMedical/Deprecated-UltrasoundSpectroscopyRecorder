/*=========================================================================

Library:   UltrasoundSpectroscopyRecorder

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/

#include "myWindow.hxx"

// Qt includes
#include <QApplication>
#include <QDebug>

// VTK includes
#include <vtkNew.h>

// STD includes
#include <iostream>


int main(int argc,char *argv[])
{
  qDebug() << "Starting ...";
  QApplication app(argc,argv);
  myWindow window;
  
  bool printHelp(false);
  bool renderingOff(false);
  std::string inputConfigFileName;
  double depthCm = -1;
  double dynRangeDb = -1;
  double frequencyMhz = -1;
  bool outputCompressed(false);
  std::string outputVideoBufferSequenceFileNameBmode;
  std::string outputVideoBufferSequenceFileNameRfmode;

  std::string acqMode("B");

  vtksys::CommandLineArguments args;
  args.Initialize(argc, argv);

  int verboseLevel = vtkPlusLogger::LOG_LEVEL_UNDEFINED;

  args.AddArgument("--help", vtksys::CommandLineArguments::NO_ARGUMENT, &printHelp, "Print this help.");
  args.AddArgument("--config-file", vtksys::CommandLineArguments::EQUAL_ARGUMENT, &inputConfigFileName, "Config file containing the device configuration.");
  args.AddArgument("--acq-mode", vtksys::CommandLineArguments::EQUAL_ARGUMENT, &acqMode, "Acquisition mode: B or RF (Default: B).");
  args.AddArgument("--depth", vtksys::CommandLineArguments::EQUAL_ARGUMENT, &depthCm, "Depth in cm.");
  args.AddArgument("--frequencyMhz", vtksys::CommandLineArguments::EQUAL_ARGUMENT, &frequencyMhz, "Frequency in MHz");
  args.AddArgument("--dynRangeDb", vtksys::CommandLineArguments::EQUAL_ARGUMENT, &dynRangeDb, "BMode Dynamic Range. 1 corresponds to the maximum dynamic range.");
  args.AddArgument("--rendering-off", vtksys::CommandLineArguments::NO_ARGUMENT, &renderingOff, "Run test without rendering.");
  args.AddArgument("--verbose", vtksys::CommandLineArguments::EQUAL_ARGUMENT, &verboseLevel, "Verbose level 1=error only, 2=warning, 3=info, 4=debug, 5=trace)");
  args.AddArgument("--output-video-buffer-seq-file", vtksys::CommandLineArguments::EQUAL_ARGUMENT, &outputVideoBufferSequenceFileNameBmode, "Filename of the output video buffer sequence metafile (B mode) (Default: VideoBufferMetafile)");
  args.AddArgument("--output-compressed", vtksys::CommandLineArguments::EQUAL_ARGUMENT, &outputCompressed, "Compressed output (0=non-compressed, 1=compressed, default:compressed)");

  if (!args.Parse())
  {
	  std::cerr << "Problem parsing arguments" << std::endl;
	  std::cout << "\n\nvtkPlusIntersonSDKCxxVideoSourceTest help:" << args.GetHelp() << std::endl;
	  exit(EXIT_FAILURE);
  }

  vtkPlusLogger::Instance()->SetLogLevel(verboseLevel);

  if (printHelp)
  {
	  std::cout << "\n\nvtkPlusIntersonSDKCxxVideoSourceTest help:" << args.GetHelp() << std::endl;
	  exit(EXIT_SUCCESS);
  }

  vtkNew<vtkIntersonSDKCxxVideoSourceWindow> intersonDevice;
  intersonDevice->SetDeviceId("VideoDevice");
  intersonDevice->SetRenderingOff(renderingOff);
  intersonDevice->SetOutputCompressed(outputCompressed);

  // Read config file
  if (STRCASECMP(inputConfigFileName.c_str(), "") != 0)
  {
	  LOG_DEBUG("Reading config file...");
	  vtkSmartPointer<vtkXMLDataElement> configRootElement = vtkSmartPointer<vtkXMLDataElement>::New();
	  if (PlusXmlUtils::ReadDeviceSetConfigurationFromFile(configRootElement, inputConfigFileName.c_str()) == PLUS_FAIL)
	  {
		  LOG_ERROR("Unable to read configuration from file " << inputConfigFileName.c_str());
		  return EXIT_FAILURE;
	  }
	  intersonDevice->ReadConfiguration(configRootElement);
  }

  if (STRCASECMP(acqMode.c_str(), "B") == 0)
  {
	  LOG_DEBUG("Acquisition mode: B");
  }
  else if (STRCASECMP(acqMode.c_str(), "RF") == 0)
  {
	  LOG_DEBUG("Acquisition mode: RF");
	  LOG_ERROR("This program only support the B mode acquisition");
	  return EXIT_FAILURE;
  }
  else
  {
	  LOG_ERROR("Unsupported Acquisition mode requested: " << acqMode);
	  exit(EXIT_FAILURE);
  }

  window.SetIntersonDevice(intersonDevice.GetPointer());

  LOG_INFO("Draw the window");
  window.show();

  LOG_INFO("Start the main application");
  return app.exec();
}
