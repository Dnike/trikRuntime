/* Copyright 2013 - 2015 Matvey Bryksin, Yurii Litvinov and CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QScopedPointer>
#include <QtCore/QThread>
#include <QtCore/QHash>
#include <QtCore/QSet>

#include <trikKernel/configurer.h>

#include "gamepadInterface.h"

namespace trikNetwork {

class TcpConnector;

/// Implementation of remote control interface.
class Gamepad : public GamepadInterface
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param port - TCP port of a gamepad server.
	Gamepad(int port);

	/// Constructor.
	/// @param configurer - configurer object that contains preparsed XML config.
	Gamepad(trikKernel::Configurer const &configurer);

	/// Destructor declared here for QScopedPointer to be able to clean up forward-declared TcpConnector.
	~Gamepad() override;

public slots:
	void reset() override;

	bool buttonWasPressed(int buttonNumber) override;

	bool isPadPressed(int pad) override;

	int padX(int pad) override;

	int padY(int pad) override;

private slots:
	void parse(QString const &message);

private:
	Q_DISABLE_COPY(Gamepad)

	/// Does object initialization.
	void init(int port);

	struct PadStatus {
		int x;
		int y;
		bool isPressed;
	};

	QScopedPointer<TcpConnector> mListener;
	QThread mNetworkThread;

	QSet<int> mButtonWasPressed;
	QHash<int, PadStatus> mPads;
};

}
