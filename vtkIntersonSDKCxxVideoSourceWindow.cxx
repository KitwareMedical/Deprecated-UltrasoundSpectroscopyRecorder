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

#include <QMainWindow>

#include "PlusConfigure.h"
#include "vtkIntersonSDKCxxVideoSourceWindow.h"

#include "vtkCommand.h"
#include "vtkImageViewer2.h"

#include "vtkPlusChannel.h"
#include "vtkPlusDataSource.h"
#include "vtkPlusRfToBrightnessConvert.h"

#include "vtkInteractorStyleImage.h"
#include "vtkInteractorStyle.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkIntersonSDKCxxVideoSourceWindow);

//----------------------------------------------------------------------------
std::string vtkIntersonSDKCxxVideoSourceWindow::GetOutputVideoBufferSequenceFileNameRfmode() const
{
	return this->outputVideoBufferSequenceFileNameRfmode;
}

//----------------------------------------------------------------------------
bool vtkIntersonSDKCxxVideoSourceWindow::GetRenderingOff() const
{
	return this->renderingOff;
}

//----------------------------------------------------------------------------
bool vtkIntersonSDKCxxVideoSourceWindow::GetOutputCompressed() const
{
	return this->outputCompressed;
}

//----------------------------------------------------------------------------
std::string vtkIntersonSDKCxxVideoSourceWindow::GetInputConfigFileName() const
{
	return this->inputConfigFileName;
}

//----------------------------------------------------------------------------
int vtkIntersonSDKCxxVideoSourceWindow::GetPulseMin() const
{
	return this->pulseMin;
}

//----------------------------------------------------------------------------
int vtkIntersonSDKCxxVideoSourceWindow::GetPulseMax() const
{
	return this->pulseMax;
}

//----------------------------------------------------------------------------
int vtkIntersonSDKCxxVideoSourceWindow::GetPulseStep() const
{
	return this->pulseStep;
}

//----------------------------------------------------------------------------
double vtkIntersonSDKCxxVideoSourceWindow::GetFrequency() const
{
	return this->frequency;
}

//----------------------------------------------------------------------------
void vtkIntersonSDKCxxVideoSourceWindow::SetOutputVideoBufferSequenceFileNameRfmode(std::string fileName)
{
	this->outputVideoBufferSequenceFileNameRfmode = fileName;
}

//----------------------------------------------------------------------------
void vtkIntersonSDKCxxVideoSourceWindow::SetRenderingOff(bool renderingOff)
{
	this->renderingOff = renderingOff;
}

//----------------------------------------------------------------------------
void vtkIntersonSDKCxxVideoSourceWindow::SetOutputCompressed(bool compression)
{
	this->outputCompressed = compression;
}

//----------------------------------------------------------------------------
void vtkIntersonSDKCxxVideoSourceWindow::SetInputConfigFileName(std::string inputConfigFileName)
{
	this->inputConfigFileName = inputConfigFileName;
}

//----------------------------------------------------------------------------
void vtkIntersonSDKCxxVideoSourceWindow::SetPulseMin(int pulse)
{
	this->pulseMin = pulse;
}

//----------------------------------------------------------------------------
void vtkIntersonSDKCxxVideoSourceWindow::SetPulseMax(int pulse)
{
	this->pulseMax = pulse;
}

//----------------------------------------------------------------------------
void vtkIntersonSDKCxxVideoSourceWindow::SetPulseStep(int step)
{
	this->pulseStep = step;
}

//----------------------------------------------------------------------------
void vtkIntersonSDKCxxVideoSourceWindow::SetFrequency(double frequency)
{
	this->frequency = frequency;
}

