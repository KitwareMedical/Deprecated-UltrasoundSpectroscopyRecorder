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

#ifndef __vtkIntersonSDKCxxVideoSourceWindow_h
#define __vtkIntersonSDKCxxVideoSourceeWindow_h

#include "vtkPlusIntersonArraySDKCxxVideoSource.h"
#include "ui_UltrasoundSpectroscopyRecorder.h"
#include <QTimer>

class vtkIntersonSDKCxxVideoSourceWindow : public vtkPlusIntersonArraySDKCxxVideoSource
{
public:
	static vtkIntersonSDKCxxVideoSourceWindow *New();

	//getter
	std::string GetOutputVideoBufferSequenceFileNameRfmode() const;
	bool GetRenderingOff() const;
	bool GetOutputCompressed() const;
	std::string GetInputConfigFileName() const;
	int GetPulseMin() const;
	int GetPulseMax() const;
	int GetPulseStep() const;
	double GetFrequency() const;

	//setter
	void SetOutputVideoBufferSequenceFileNameRfmode(std::string fileName);
	void SetRenderingOff(bool renderingOff);
	void SetOutputCompressed(bool compression);
	void SetInputConfigFileName(std::string inputConfigFileName);
	void SetPulseMin(int pulse);
	void SetPulseMax(int pulse);
	void SetPulseStep(int step);
	void SetFrequency(double frequency);

private:
	std::string outputVideoBufferSequenceFileNameRfmode;
	bool renderingOff;
	bool outputCompressed;
	std::string inputConfigFileName;

	int pulseMax;
	int pulseMin;
	int pulseStep;
	double frequency;
};



#endif