/* Copyright 2013 Roman Kurbatov, Yurii Litvinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QScopedPointer>

#include "networkStructs.h"

#include "declSpec.h"

namespace trikWiFi {

class TrikWiFiWorker;

/// Class for WiFi management. Allows to connect and disconnect to/from WPA-PSK network (using wpa-supplicant utility),
/// to scan for available networks and to alter configuration of wpa-supplicant.
class TRIKWIFI_EXPORT TrikWiFi : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param interfaceFilePrefix - path and prefix of file names that are used by this library to communicate
	///        with wpa_supplicant.
	/// @param daemonFile - file that wpa_supplicant uses to communicate with clients, something like
	///        /run/wpa_supplicant/wlan0.
	/// @param parent - parent QObject.
	TrikWiFi(const QString &interfaceFilePrefix, const QString &daemonFile, QObject *parent = nullptr);

	/// Destructor.
	~TrikWiFi() override;

	/// Connect to a network with given id. Available ids can be obtained by listNetworks method.
	void connect(int id);

	/// Disconnect from network if we are currently connected to one.
	void disconnect();

	/// Asynchronously gets conection status and connection information such as SSID and IP. Emits statusReady when
	/// done, then status can be obtained by statusResult() call.
	void statusRequest();

	/// Returns last known connection status. To refresh, use statusRequest() method.
	Status statusResult() const;

	/// Asynchronously scans for available WiFi networks. When done, sends scanFinished() signal, then scan results
	/// can be obtained via scanResult() method.
	void scanRequest();

	/// Returns a list of currently known available WiFi networks. Use scanRequest() method to refresh.
	QList<ScanResult> scanResult() const;

	/// Asynchronously gets registered networks from wpa_supplicant. When ready, listNetworksReady() signal and results
	/// can be received by listNetworksResult() method. wpa_supplicant can connect only to registered networks.
	void listNetworksRequest();

	/// Returns a current list of registered networks. Use listNetworksRequest() method to refresh.
	QList<NetworkConfiguration> listNetworksResult() const;

	/// Disposes an old connection to wpa_supplicant and creates a new one.
	void reinit();

	/// Closes the connection to wpa_supplicant.
	void dispose();

signals:
	/// Emitted when scanning for available networks initiated by scan() is finished and results are available
	/// and ready to be obtained by scanResults() method.
	void scanFinished();

	/// Emitted when wpa_supplicant connects to WiFi network. SSID of this network can be retrieved by status() method.
	void connected();

	/// Emitted when wpa_supplicant disconnects from current network.
	void disconnected();

	/// Emitted when connection status requested by statusRequest() is ready and results can be obtained by
	/// statusResult() method.
	void statusReady();

	/// Emitted when list of known networks requested by listNetworksRequest() is ready and its results can be obtained
	/// by listNetworksResult() method.
	void listNetworksReady();

	/// Emitted when something goes wrong.
	void error(const QString &message);

private:
	/// Worker in separate thread that communicates with wpa_supplicant.
	QScopedPointer<TrikWiFiWorker> mWorker;

	/// Thread for worker.
	QThread mWorkerThread;
};

}
