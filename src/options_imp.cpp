/*
 * Bittorrent Client using Qt4 and libtorrent.
 * Copyright (C) 2006  Christophe Dumez
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * In addition, as a special exception, the copyright holders give permission to
 * link this program with the OpenSSL project's "OpenSSL" library (or with
 * modified versions of it that use the same license as the "OpenSSL" library),
 * and distribute the linked executables. You must obey the GNU General Public
 * License in all respects for all of the code used other than "OpenSSL".  If you
 * modify file(s), you may extend this exception to your version of the file(s),
 * but you are not obligated to do so. If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * Contact : chris@qbittorrent.org
 */
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QSettings>
#include <QPlastiqueStyle>
#include "qgnomelook.h"
#include <QMotifStyle>
#include <QCDEStyle>
#include <QDialogButtonBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#ifdef Q_WS_WIN
#include <QWindowsXPStyle>
#endif

#ifdef Q_WS_MAC
#include <QMacStyle>
#endif

#include <time.h>
#include <stdlib.h>

#include "options_imp.h"
#include "preferences.h"
#include "misc.h"

// Constructor
options_imp::options_imp(QWidget *parent):QDialog(parent){
  qDebug("-> Constructing Options");
  setAttribute(Qt::WA_DeleteOnClose);
  QString savePath;
  setupUi(this);
  // Get apply button in button box
  QList<QAbstractButton *> buttons = buttonBox->buttons();
  foreach(QAbstractButton *button, buttons){
    if(buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole){
      applyButton = button;
      break;
    }
  }
  connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(applySettings(QAbstractButton*)));
#ifdef Q_WS_WIN
  comboStyle->addItem("Windows XP Style (Windows Only)");
#endif
#ifdef Q_WS_MAC
  comboStyle->addItem("MacOS Style (MacOS Only)");
#endif
  // Languages supported
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/united_kingdom.png"))), QString::fromUtf8("English"));
  locales << "en_GB";
  comboI18n->setCurrentIndex(0);
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/france.png"))), QString::fromUtf8("Français"));
  locales << "fr_FR";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/germany.png"))), QString::fromUtf8("Deutsch"));
  locales << "de_DE";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/hungary.png"))), QString::fromUtf8("Magyar"));
  locales << "hu_HU";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/italy.png"))), QString::fromUtf8("Italiano"));
  locales << "it_IT";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/netherlands.png"))), QString::fromUtf8("Nederlands"));
  locales << "nl_NL";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/spain.png"))), QString::fromUtf8("Español"));
  locales << "es_ES";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/spain_catalunya.png"))), QString::fromUtf8("Català"));
  locales << "ca_ES";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/portugal.png"))), QString::fromUtf8("Português"));
  locales << "pt_PT";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/brazil.png"))), QString::fromUtf8("Português brasileiro"));
  locales << "pt_BR";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/poland.png"))), QString::fromUtf8("Polski"));
  locales << "pl_PL";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/czech.png"))), QString::fromUtf8("Čeština"));
  locales << "cs_CZ";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/slovakia.png"))), QString::fromUtf8("Slovenčina"));
  locales << "sk_SK";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/serbia.png"))), QString::fromUtf8("Српски"));
  locales << "sr_CS";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/romania.png"))), QString::fromUtf8("Română"));
  locales << "ro_RO";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/turkey.png"))), QString::fromUtf8("Türkçe"));
  locales << "tr_TR";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/greece.png"))), QString::fromUtf8("Ελληνικά"));
  locales << "el_GR";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/sweden.png"))), QString::fromUtf8("Svenska"));
  locales << "sv_SE";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/finland.png"))), QString::fromUtf8("Suomi"));
  locales << "fi_FI";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/norway.png"))), QString::fromUtf8("Norsk"));
  locales << "nb_NO";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/denmark.png"))), QString::fromUtf8("Dansk"));
  locales << "da_DK";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/bulgaria.png"))), QString::fromUtf8("Български"));
  locales << "bg_BG";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/ukraine.png"))), QString::fromUtf8("Українська"));
  locales << "uk_UA";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/russia.png"))), QString::fromUtf8("Русский"));
  locales << "ru_RU";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/japan.png"))), QString::fromUtf8("日本語"));
  locales << "ja_JP";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/china.png"))), QString::fromUtf8("中文 (简体)"));
  locales << "zh_CN";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/taiwan.png"))), QString::fromUtf8("中文 (繁體)"));
  locales << "zh_TW";
  comboI18n->addItem((QIcon(QString::fromUtf8(":/Icons/flags/south_korea.png"))), QString::fromUtf8("한글"));
  locales << "ko_KR";

  // Load options
  loadOptions();
  // Disable systray integration if it is not supported by the system
  if(!QSystemTrayIcon::isSystemTrayAvailable()){
    checkNoSystray->setEnabled(false);
  }
  // Connect signals / slots
  // General tab
  connect(checkNoSystray, SIGNAL(toggled(bool)), this, SLOT(setSystrayOptionsState(bool)));
  // Downloads tab
  connect(checkTempFolder, SIGNAL(toggled(bool)), this, SLOT(enableTempPathInput(bool)));
  connect(checkScanDir, SIGNAL(toggled(bool)), this, SLOT(enableDirScan(bool)));
  connect(actionTorrentDlOnDblClBox, SIGNAL(currentIndexChanged(int)), this, SLOT(enableApplyButton()));
  connect(actionTorrentFnOnDblClBox, SIGNAL(currentIndexChanged(int)), this, SLOT(enableApplyButton()));
  connect(checkTempFolder, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  // Connection tab
  connect(checkUploadLimit, SIGNAL(toggled(bool)), this, SLOT(enableUploadLimit(bool)));
  connect(checkDownloadLimit,  SIGNAL(toggled(bool)), this, SLOT(enableDownloadLimit(bool)));
  // Bittorrent tab
  connect(checkMaxConnecs,  SIGNAL(toggled(bool)), this, SLOT(enableMaxConnecsLimit(bool)));
  connect(checkMaxConnecsPerTorrent,  SIGNAL(toggled(bool)), this, SLOT(enableMaxConnecsLimitPerTorrent(bool)));
  connect(checkMaxUploadsPerTorrent,  SIGNAL(toggled(bool)), this, SLOT(enableMaxUploadsLimitPerTorrent(bool)));
  connect(checkRatioLimit,  SIGNAL(toggled(bool)), this, SLOT(enableShareRatio(bool)));
  connect(checkRatioRemove,  SIGNAL(toggled(bool)), this, SLOT(enableDeleteRatio(bool)));
  connect(checkDHT, SIGNAL(toggled(bool)), this, SLOT(enableDHTSettings(bool)));
  connect(checkDifferentDHTPort, SIGNAL(toggled(bool)), this, SLOT(enableDHTPortSettings(bool)));
  // Proxy tab
  connect(comboProxyType_http, SIGNAL(currentIndexChanged(int)),this, SLOT(enableProxyHTTP(int)));
  connect(checkProxyAuth_http,  SIGNAL(toggled(bool)), this, SLOT(enableProxyAuthHTTP(bool)));
  connect(comboProxyType, SIGNAL(currentIndexChanged(int)),this, SLOT(enableProxy(int)));
  connect(checkProxyAuth,  SIGNAL(toggled(bool)), this, SLOT(enableProxyAuth(bool)));
  // Misc tab
  connect(checkIPFilter, SIGNAL(toggled(bool)), this, SLOT(enableFilter(bool)));
  connect(checkEnableRSS, SIGNAL(toggled(bool)), this, SLOT(enableRSS(bool)));
  connect(checkEnableQueueing, SIGNAL(toggled(bool)), this, SLOT(enableQueueingSystem(bool)));
  // Web UI tab
  connect(checkWebUi,  SIGNAL(toggled(bool)), this, SLOT(enableWebUi(bool)));

  // Apply button is activated when a value is changed
  // General tab
  connect(comboI18n, SIGNAL(currentIndexChanged(int)), this, SLOT(enableApplyButton()));
  connect(comboStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(enableApplyButton()));
  connect(checkConfirmExit, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkSpeedInTitle, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(spinRefreshInterval, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  connect(checkNoSystray, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkCloseToSystray, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkMinimizeToSysTray, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkStartMinimized, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkSystrayBalloons, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkDisplayToolbar, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkNoSplash, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  // Downloads tab
  connect(textSavePath, SIGNAL(textChanged(QString)), this, SLOT(enableApplyButton()));
  connect(checkPreallocateAll, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkAdditionDialog, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkStartPaused, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkScanDir, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(textScanDir, SIGNAL(textChanged(QString)), this, SLOT(enableApplyButton()));
  // Connection tab
  connect(spinPort, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  connect(checkUPnP, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkNATPMP, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkUploadLimit, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkDownloadLimit, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(spinUploadLimit, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  connect(spinDownloadLimit, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  connect(checkResolveCountries, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkResolveHosts, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  // Bittorrent tab
  connect(checkMaxConnecs, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkMaxConnecsPerTorrent, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkMaxUploadsPerTorrent, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(spinMaxConnec, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  connect(spinMaxConnecPerTorrent, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  connect(spinMaxUploadsPerTorrent, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  connect(checkDHT, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkDifferentDHTPort, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(spinDHTPort, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  connect(checkLSD, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkAzureusSpoof, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(comboEncryption, SIGNAL(currentIndexChanged(int)), this, SLOT(enableApplyButton()));
  connect(checkRatioLimit, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkRatioRemove, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(spinRatio, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  connect(spinMaxRatio, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  // Proxy tab
  connect(comboProxyType_http, SIGNAL(currentIndexChanged(int)), this, SLOT(enableApplyButton()));
  connect(textProxyIP_http, SIGNAL(textChanged(QString)), this, SLOT(enableApplyButton()));
  connect(spinProxyPort_http, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  connect(checkProxyAuth_http, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(textProxyUsername_http, SIGNAL(textChanged(QString)), this, SLOT(enableApplyButton()));
  connect(textProxyPassword_http, SIGNAL(textChanged(QString)), this, SLOT(enableApplyButton()));
  connect(comboProxyType, SIGNAL(currentIndexChanged(int)), this, SLOT(enableApplyButton()));
  connect(textProxyIP, SIGNAL(textChanged(QString)), this, SLOT(enableApplyButton()));
  connect(spinProxyPort, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  connect(checkProxyAuth, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(textProxyUsername, SIGNAL(textChanged(QString)), this, SLOT(enableApplyButton()));
  connect(textProxyPassword, SIGNAL(textChanged(QString)), this, SLOT(enableApplyButton()));
  connect(checkProxyTrackers, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkProxyPeers, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkProxyWebseeds, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkProxyDHT, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  // Misc tab
  connect(checkIPFilter, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(textFilterPath, SIGNAL(textChanged(QString)), this, SLOT(enableApplyButton()));
  connect(spinRSSRefresh, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  connect(spinRSSMaxArticlesPerFeed, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  connect(checkEnableRSS, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(checkEnableQueueing, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(spinMaxActiveDownloads, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  connect(spinMaxActiveUploads, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  connect(spinMaxActiveTorrents, SIGNAL(valueChanged(QString)), this, SLOT(enableApplyButton()));
  // Web UI tab
  connect(checkWebUi, SIGNAL(toggled(bool)), this, SLOT(enableApplyButton()));
  connect(spinWebUiPort, SIGNAL(valueChanged(int)), this, SLOT(enableApplyButton()));
  connect(textWebUiUsername, SIGNAL(textChanged(QString)), this, SLOT(enableApplyButton()));
  connect(textWebUiPassword, SIGNAL(textChanged(QString)), this, SLOT(enableApplyButton()));
  // Disable apply Button
  applyButton->setEnabled(false);
  if(!QSystemTrayIcon::supportsMessages()){
    // Mac OS X doesn't support it yet
    checkSystrayBalloons->setChecked(false);
    checkSystrayBalloons->setEnabled(false);
  }
  // Tab selection mecanism
  connect(tabSelection, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
  // Adapt size
  loadWindowState();
  show();
}

// Main destructor
options_imp::~options_imp(){
  qDebug("-> destructing Options");
}

void options_imp::changePage(QListWidgetItem *current, QListWidgetItem *previous) {
  if (!current)
    current = previous;
  tabOption->setCurrentIndex(tabSelection->row(current));
}

void options_imp::useStyle(){
  int style = getStyle();
  switch(style) {
  case 1:
    QApplication::setStyle(new QPlastiqueStyle());
    break;
  case 2:
    QApplication::setStyle(new QGnomeLookStyle());
    break;
  case 3:
    QApplication::setStyle(new QMotifStyle());
    break;
  case 4:
    QApplication::setStyle(new QCDEStyle());
    break;
#ifdef Q_WS_MAC
  case 5:
    QApplication::setStyle(new QMacStyle());
    break;
#endif
#ifdef Q_WS_WIN
  case 6:
    QApplication::setStyle(new QWindowsXPStyle());
    break;
#endif
  }
}

void options_imp::loadWindowState() {
  QSettings settings(QString::fromUtf8("qBittorrent"), QString::fromUtf8("qBittorrent"));
  resize(settings.value(QString::fromUtf8("Preferences/State/size"), sizeFittingScreen()).toSize());
  QPoint p = settings.value(QString::fromUtf8("Preferences/State/pos"), QPoint()).toPoint();
  if(!p.isNull())
    move(p);
}

void options_imp::saveWindowState() const {
  QSettings settings(QString::fromUtf8("qBittorrent"), QString::fromUtf8("qBittorrent"));
  settings.setValue(QString::fromUtf8("Preferences/State/size"), size());
  settings.setValue(QString::fromUtf8("Preferences/State/pos"), pos());
}

QSize options_imp::sizeFittingScreen() {
  int scrn = 0;
  QWidget *w = this->topLevelWidget();

  if(w)
    scrn = QApplication::desktop()->screenNumber(w);
  else if(QApplication::desktop()->isVirtualDesktop())
    scrn = QApplication::desktop()->screenNumber(QCursor::pos());
  else
    scrn = QApplication::desktop()->screenNumber(this);

  QRect desk(QApplication::desktop()->availableGeometry(scrn));
  if(width() > desk.width() || height() > desk.height()) {
    if(desk.width() > 0 && desk.height() > 0)
      return QSize(desk.width(), desk.height());
  }
  return size();
}

void options_imp::saveOptions(){
  applyButton->setEnabled(false);
  QSettings settings("qBittorrent", "qBittorrent");
  // Apply style
  useStyle();
  settings.beginGroup("Preferences");
  // General preferences
  settings.beginGroup("General");
  settings.setValue(QString::fromUtf8("Locale"), getLocale());
  settings.setValue(QString::fromUtf8("Style"), getStyle());
  settings.setValue(QString::fromUtf8("ExitConfirm"), confirmOnExit());
  settings.setValue(QString::fromUtf8("SpeedInTitleBar"), speedInTitleBar());
  settings.setValue(QString::fromUtf8("RefreshInterval"), getRefreshInterval());
  settings.setValue(QString::fromUtf8("SystrayEnabled"), systrayIntegration());
  settings.setValue(QString::fromUtf8("CloseToTray"), closeToTray());
  settings.setValue(QString::fromUtf8("MinimizeToTray"), minimizeToTray());
  settings.setValue(QString::fromUtf8("StartMinimized"), startMinimized());
  settings.setValue(QString::fromUtf8("NotificationBaloons"), OSDEnabled());
  settings.setValue(QString::fromUtf8("ToolbarDisplayed"), isToolbarDisplayed());
  settings.setValue(QString::fromUtf8("NoSplashScreen"), isSlashScreenDisabled());
  // End General preferences
  settings.endGroup();
  // Downloads preferences
  settings.beginGroup("Downloads");
  settings.setValue(QString::fromUtf8("SavePath"), getSavePath());
  settings.setValue(QString::fromUtf8("TempPathEnabled"), isTempPathEnabled());
  settings.setValue(QString::fromUtf8("TempPath"), getTempPath());
  settings.setValue(QString::fromUtf8("PreAllocation"), preAllocateAllFiles());
  settings.setValue(QString::fromUtf8("AdditionDialog"), useAdditionDialog());
  settings.setValue(QString::fromUtf8("StartInPause"), addTorrentsInPause());
  settings.setValue(QString::fromUtf8("ScanDir"), getScanDir());
  settings.setValue(QString::fromUtf8("DblClOnTorDl"), getActionOnDblClOnTorrentDl());
  settings.setValue(QString::fromUtf8("DblClOnTorFn"), getActionOnDblClOnTorrentFn());
  // End Downloads preferences
  settings.endGroup();
  // Connection preferences
  settings.beginGroup("Connection");
  settings.setValue(QString::fromUtf8("PortRangeMin"), getPort());
  settings.setValue(QString::fromUtf8("UPnP"), isUPnPEnabled());
  settings.setValue(QString::fromUtf8("NAT-PMP"), isNATPMPEnabled());
  settings.setValue(QString::fromUtf8("GlobalDLLimit"), getGlobalBandwidthLimits().first);
  settings.setValue(QString::fromUtf8("GlobalUPLimit"), getGlobalBandwidthLimits().second);
  settings.setValue("ResolvePeerCountries", checkResolveCountries->isChecked());
  settings.setValue("ResolvePeerHostNames", checkResolveHosts->isChecked());
  settings.setValue(QString::fromUtf8("ProxyType"), getProxyType());
  //if(isProxyEnabled()) {
  settings.beginGroup("Proxy");
  // Proxy is enabled, save settings
  settings.setValue(QString::fromUtf8("IP"), getProxyIp());
  settings.setValue(QString::fromUtf8("Port"), getProxyPort());
  settings.setValue(QString::fromUtf8("Authentication"), isProxyAuthEnabled());
  //if(isProxyAuthEnabled()) {
  // Credentials
  settings.setValue(QString::fromUtf8("Username"), getProxyUsername());
  settings.setValue(QString::fromUtf8("Password"), getProxyPassword());
  //}
  // Affected connections
  settings.setValue(QString::fromUtf8("AffectTrackers"), useProxyForTrackers());
  settings.setValue(QString::fromUtf8("AffectPeers"), useProxyForPeers());
  settings.setValue(QString::fromUtf8("AffectWebSeeds"), useProxyForWebseeds());
  settings.setValue(QString::fromUtf8("AffectDHT"), useProxyForDHT());
  settings.endGroup(); // End Proxy
  //}
  settings.setValue(QString::fromUtf8("HTTPProxyType"), getHTTPProxyType());
  //if(isHTTPProxyEnabled()) {
  settings.beginGroup("HTTPProxy");
  // Proxy is enabled, save settings
  settings.setValue(QString::fromUtf8("IP"), getHTTPProxyIp());
  settings.setValue(QString::fromUtf8("Port"), getHTTPProxyPort());
  settings.setValue(QString::fromUtf8("Authentication"), isHTTPProxyAuthEnabled());
  //if(isHTTPProxyAuthEnabled()) {
  // Credentials
  settings.setValue(QString::fromUtf8("Username"), getHTTPProxyUsername());
  settings.setValue(QString::fromUtf8("Password"), getHTTPProxyPassword());
  //}
  settings.endGroup(); // End HTTPProxy
  //}
  // End Connection preferences
  settings.endGroup();
  // Bittorrent preferences
  settings.beginGroup("Bittorrent");
  settings.setValue(QString::fromUtf8("MaxConnecs"), getMaxConnecs());
  settings.setValue(QString::fromUtf8("MaxConnecsPerTorrent"), getMaxConnecsPerTorrent());
  settings.setValue(QString::fromUtf8("MaxUploadsPerTorrent"), getMaxUploadsPerTorrent());
  settings.setValue(QString::fromUtf8("DHT"), isDHTEnabled());
  settings.setValue(QString::fromUtf8("sameDHTPortAsBT"), isDHTPortSameAsBT());
  settings.setValue(QString::fromUtf8("DHTPort"), getDHTPort());
  settings.setValue(QString::fromUtf8("LSD"), isLSDEnabled());
  settings.setValue(QString::fromUtf8("utorrentSpoof"), isUtorrentSpoofingEnabled());
  settings.setValue(QString::fromUtf8("Encryption"), getEncryptionSetting());
  settings.setValue(QString::fromUtf8("DesiredRatio"), getDesiredRatio());
  settings.setValue(QString::fromUtf8("MaxRatio"), getDeleteRatio());
  // End Bittorrent preferences
  settings.endGroup();
  // Misc preferences
  // * IPFilter
  settings.beginGroup("IPFilter");
  settings.setValue(QString::fromUtf8("Enabled"), isFilteringEnabled());
  if(isFilteringEnabled()){
    settings.setValue(QString::fromUtf8("File"), textFilterPath->text());
  }
  // End IPFilter preferences
  settings.endGroup();
  // RSS
  settings.beginGroup("RSS");
  settings.setValue(QString::fromUtf8("RSSEnabled"), isRSSEnabled());
  settings.setValue(QString::fromUtf8("RSSRefresh"), spinRSSRefresh->value());
  settings.setValue(QString::fromUtf8("RSSMaxArticlesPerFeed"), spinRSSMaxArticlesPerFeed->value());
  // End RSS preferences
  settings.endGroup();
  // Queueing system
  settings.beginGroup("Queueing");
  settings.setValue(QString::fromUtf8("QueueingEnabled"), isQueueingSystemEnabled());
  settings.setValue(QString::fromUtf8("MaxActiveDownloads"), spinMaxActiveDownloads->value());
  settings.setValue(QString::fromUtf8("MaxActiveUploads"), spinMaxActiveUploads->value());
  settings.setValue(QString::fromUtf8("MaxActiveTorrents"), spinMaxActiveTorrents->value());
  // End Queueing system preferences
  settings.endGroup();
  // Web UI
  settings.beginGroup("WebUI");
  settings.setValue("Enabled", isWebUiEnabled());
  if(isWebUiEnabled())
  {
    settings.setValue("Port", webUiPort());
    settings.setValue("Username", webUiUsername());
    // FIXME: Check that the password is valid (not empty at least)
    Preferences::setWebUiPassword(webUiPassword());
  }
  // End Web UI
  settings.endGroup();
  // End preferences
  settings.endGroup();
}

bool options_imp::isUtorrentSpoofingEnabled() const {
  return checkAzureusSpoof->isChecked();
}

bool options_imp::isFilteringEnabled() const{
  return checkIPFilter->isChecked();
}

int options_imp::getProxyType() const{
  if(comboProxyType->currentIndex() == HTTP){
    if(isProxyAuthEnabled()){
      return HTTP_PW;
    }else{
      return HTTP;
    }
  }else{
    if(comboProxyType->currentIndex() == SOCKS5){
      if(isProxyAuthEnabled()){
        return SOCKS5_PW;
      }else{
        return SOCKS5;
      }
    }
  }
  return -1; // disabled
}

int options_imp::getHTTPProxyType() const {
  if(comboProxyType_http->currentIndex() == HTTP){
    if(isHTTPProxyAuthEnabled()){
      return HTTP_PW;
    }else{
      return HTTP;
    }
  }
  return -1; // disabled
}

bool options_imp::useProxyForTrackers() const{
  return checkProxyTrackers->isChecked();
}

bool options_imp::useProxyForPeers() const{
  return checkProxyPeers->isChecked();
}

bool options_imp::useProxyForWebseeds() const{
  return checkProxyWebseeds->isChecked();
}

bool options_imp::useProxyForDHT() const{
  return checkProxyDHT->isChecked();
}

int options_imp::getStyle() const{
  return comboStyle->currentIndex();
}

void options_imp::setStyle(int style){
  if(style >= comboStyle->count() || style < 0)
    style = 0;
  comboStyle->setCurrentIndex(style);
}

bool options_imp::isHTTPProxyAuthEnabled() const{
  return checkProxyAuth_http->isChecked();
}

void options_imp::loadOptions(){
  int intValue;
  float floatValue;
  QString strValue;
  // General preferences
  setLocale(Preferences::getLocale());
  setStyle(Preferences::getStyle());
  checkConfirmExit->setChecked(Preferences::confirmOnExit());
  checkSpeedInTitle->setChecked(Preferences::speedInTitleBar());
  spinRefreshInterval->setValue(Preferences::getRefreshInterval());
  checkNoSystray->setChecked(!Preferences::systrayIntegration());
  checkDisplayToolbar->setChecked(Preferences::isToolbarDisplayed());
  checkNoSplash->setChecked(Preferences::isSlashScreenDisabled());
  if(checkNoSystray->isChecked()) {
    disableSystrayOptions();
  } else {
    enableSystrayOptions();
    checkCloseToSystray->setChecked(Preferences::closeToTray());
    checkMinimizeToSysTray->setChecked(Preferences::minimizeToTray());
    checkStartMinimized->setChecked(Preferences::startMinimized());
    checkSystrayBalloons->setChecked(Preferences::OSDEnabled());
  }
  // End General preferences
  // Downloads preferences
  textSavePath->setText(Preferences::getSavePath());
  if(Preferences::isTempPathEnabled()) {
    // enable
    checkTempFolder->setChecked(true);
    enableTempPathInput(checkTempFolder->isChecked());
  } else {
    checkTempFolder->setChecked(false);
    enableTempPathInput(checkTempFolder->isChecked());
  }
  textTempPath->setText(Preferences::getTempPath());
  checkPreallocateAll->setChecked(Preferences::preAllocateAllFiles());
  checkAdditionDialog->setChecked(Preferences::useAdditionDialog());
  checkStartPaused->setChecked(Preferences::addTorrentsInPause());
  strValue = Preferences::getScanDir();
  if(strValue.isEmpty()) {
    // Disable
    checkScanDir->setChecked(false);
    enableDirScan(checkScanDir->isChecked());
  } else {
    // enable
    checkScanDir->setChecked(true);
    textScanDir->setText(strValue);
    enableDirScan(checkScanDir->isChecked());
  }
  intValue = Preferences::getActionOnDblClOnTorrentDl();
  if(intValue >= actionTorrentDlOnDblClBox->count())
    intValue = 0;
  actionTorrentDlOnDblClBox->setCurrentIndex(intValue);
  intValue = Preferences::getActionOnDblClOnTorrentFn();
  if(intValue >= actionTorrentFnOnDblClBox->count())
    intValue = 1;
  actionTorrentFnOnDblClBox->setCurrentIndex(intValue);
  // End Downloads preferences
  // Connection preferences
  spinPort->setValue(Preferences::getSessionPort());
  checkUPnP->setChecked(Preferences::isUPnPEnabled());
  checkNATPMP->setChecked(Preferences::isNATPMPEnabled());
  intValue = Preferences::getGlobalDownloadLimit();
  if(intValue > 0) {
    // Enabled
    checkDownloadLimit->setChecked(true);
    spinDownloadLimit->setEnabled(true);
    spinDownloadLimit->setValue(intValue);
  } else {
    // Disabled
    checkDownloadLimit->setChecked(false);
    spinDownloadLimit->setEnabled(false);
  }
  intValue = Preferences::getGlobalUploadLimit();
  if(intValue != -1) {
    // Enabled
    checkUploadLimit->setChecked(true);
    spinUploadLimit->setEnabled(true);
    spinUploadLimit->setValue(intValue);
  } else {
    // Disabled
    checkUploadLimit->setChecked(false);
    spinUploadLimit->setEnabled(false);
  }
  // Peer connections
  checkResolveCountries->setChecked(Preferences::resolvePeerCountries());
  checkResolveHosts->setChecked(Preferences::resolvePeerHostNames());

  intValue = Preferences::getProxyType();
  if(intValue <= 0) {
    intValue = 0;
  } else {
    if(intValue%2 == 0) {
      intValue = 2;
    }else {
      intValue = 1;
    }
  }
  comboProxyType->setCurrentIndex(intValue);
  enableProxy(intValue);
  //if(isProxyEnabled()) {
  // Proxy is enabled, save settings
  textProxyIP->setText(Preferences::getProxyIp());
  spinProxyPort->setValue(Preferences::getProxyPort());
  checkProxyAuth->setChecked(Preferences::isProxyAuthEnabled());
  textProxyUsername->setText(Preferences::getProxyUsername());
  textProxyPassword->setText(Preferences::getProxyPassword());
  enableProxyAuth(checkProxyAuth->isChecked());
  // Affected connections
  checkProxyTrackers->setChecked(Preferences::useProxyForTrackers());
  checkProxyPeers->setChecked(Preferences::useProxyForPeers());
  checkProxyWebseeds->setChecked(Preferences::useProxyForWebseeds());
  checkProxyDHT->setChecked(Preferences::useProxyForDHT());
  //}
  intValue = Preferences::getHTTPProxyType();
  if(intValue <= 0) {
    intValue = 0;
  } else {
    intValue = 1;
  }
  comboProxyType_http->setCurrentIndex(intValue);
  enableProxyHTTP(intValue);
  textProxyUsername_http->setText(Preferences::getHTTPProxyUsername());
  textProxyPassword_http->setText(Preferences::getHTTPProxyPassword());
  textProxyIP_http->setText(Preferences::getHTTPProxyIp());
  spinProxyPort_http->setValue(Preferences::getHTTPProxyPort());
  checkProxyAuth_http->setChecked(Preferences::isHTTPProxyAuthEnabled());
  enableProxyAuthHTTP(checkProxyAuth_http->isChecked());
  // End HTTPProxy
  // End Connection preferences
  // Bittorrent preferences
  intValue = Preferences::getMaxConnecs();
  if(intValue > 0) {
    // enable
    checkMaxConnecs->setChecked(true);
    spinMaxConnec->setEnabled(true);
    spinMaxConnec->setValue(intValue);
  } else {
    // disable
    checkMaxConnecs->setChecked(false);
    spinMaxConnec->setEnabled(false);
  }
  intValue = Preferences::getMaxConnecsPerTorrent();
  if(intValue > 0) {
    // enable
    checkMaxConnecsPerTorrent->setChecked(true);
    spinMaxConnecPerTorrent->setEnabled(true);
    spinMaxConnecPerTorrent->setValue(intValue);
  } else {
    // disable
    checkMaxConnecsPerTorrent->setChecked(false);
    spinMaxConnecPerTorrent->setEnabled(false);
  }
  intValue = Preferences::getMaxUploadsPerTorrent();
  if(intValue > 0) {
    // enable
    checkMaxUploadsPerTorrent->setChecked(true);
    spinMaxUploadsPerTorrent->setEnabled(true);
    spinMaxUploadsPerTorrent->setValue(intValue);
  } else {
    // disable
    checkMaxUploadsPerTorrent->setChecked(false);
    spinMaxUploadsPerTorrent->setEnabled(false);
  }
  checkDHT->setChecked(Preferences::isDHTEnabled());
  enableDHTSettings(checkDHT->isChecked());
  checkDifferentDHTPort->setChecked(!Preferences::isDHTPortSameAsBT());
  enableDHTPortSettings(checkDifferentDHTPort->isChecked());
  spinDHTPort->setValue(Preferences::getDHTPort());
  checkLSD->setChecked(Preferences::isLSDEnabled());
  checkAzureusSpoof->setChecked(Preferences::isUtorrentSpoofingEnabled());
  comboEncryption->setCurrentIndex(Preferences::getEncryptionSetting());
  floatValue = Preferences::getDesiredRatio();
  if(floatValue >= 1.) {
    // Enable
    checkRatioLimit->setChecked(true);
    spinRatio->setEnabled(true);
    spinRatio->setValue(floatValue);
  } else {
    // Disable
    checkRatioLimit->setChecked(false);
    spinRatio->setEnabled(false);
  }
  floatValue = Preferences::getDeleteRatio();
  if(floatValue >= 1.) {
    // Enable
    checkRatioRemove->setChecked(true);
    spinMaxRatio->setEnabled(true);
    spinMaxRatio->setValue(floatValue);
  } else {
    // Disable
    checkRatioRemove->setChecked(false);
    spinMaxRatio->setEnabled(false);
  }
  // End Bittorrent preferences
  // Misc preferences
  // * IP Filter
  checkIPFilter->setChecked(Preferences::isFilteringEnabled());
  enableFilter(checkIPFilter->isChecked());
  textFilterPath->setText(Preferences::getFilter());
  // End IP Filter
  // * RSS
  checkEnableRSS->setChecked(Preferences::isRSSEnabled());
  enableRSS(checkEnableRSS->isChecked());
  spinRSSRefresh->setValue(Preferences::getRSSRefreshInterval());
  spinRSSMaxArticlesPerFeed->setValue(Preferences::getRSSMaxArticlesPerFeed());
  // End RSS preferences
  // Queueing system preferences
  checkEnableQueueing->setChecked(Preferences::isQueueingSystemEnabled());
  enableQueueingSystem(checkEnableQueueing->isChecked());
  spinMaxActiveDownloads->setValue(Preferences::getMaxActiveDownloads());
  spinMaxActiveUploads->setValue(Preferences::getMaxActiveUploads());
  spinMaxActiveTorrents->setValue(Preferences::getMaxActiveTorrents());
  // End Queueing system preferences
  // Web UI
  checkWebUi->setChecked(Preferences::isWebUiEnabled());
  enableWebUi(checkWebUi->isChecked());
  spinWebUiPort->setValue(Preferences::getWebUiPort());
  textWebUiUsername->setText(Preferences::getWebUiUsername());
  textWebUiPassword->setText(Preferences::getWebUiPassword());
  // End Web UI
  // Random stuff
  srand(time(0));
}

// return min & max ports
// [min, max]
int options_imp::getPort() const{
  return spinPort->value();
}

void options_imp::on_randomButton_clicked() {
  // Range [1024: 65535]
  spinPort->setValue(rand() % 64512 + 1024);
}

int options_imp::getEncryptionSetting() const{
  return comboEncryption->currentIndex();
}

int options_imp::getMaxActiveDownloads() const {
  return spinMaxActiveDownloads->value();
}

int options_imp::getMaxActiveUploads() const {
  return spinMaxActiveUploads->value();
}

int options_imp::getMaxActiveTorrents() const {
  return spinMaxActiveTorrents->value();
}

bool options_imp::minimizeToTray() const{
  if(checkNoSystray->isChecked()) return false;
  return checkMinimizeToSysTray->isChecked();
}

bool options_imp::closeToTray() const{
  if(checkNoSystray->isChecked()) return false;
  return checkCloseToSystray->isChecked();
}

unsigned int options_imp::getRefreshInterval() const {
  return spinRefreshInterval->value();
}

bool options_imp::confirmOnExit() const{
  return checkConfirmExit->isChecked();
}

bool options_imp::isDirScanEnabled() const {
  return checkScanDir->isChecked();
}

bool options_imp::isQueueingSystemEnabled() const {
  return checkEnableQueueing->isChecked();
}

bool options_imp::isDHTEnabled() const{
  return checkDHT->isChecked();
}

bool options_imp::isRSSEnabled() const{
  return checkEnableRSS->isChecked();
}

bool options_imp::isLSDEnabled() const{
  return checkLSD->isChecked();
}

bool options_imp::isUPnPEnabled() const{
  return checkUPnP->isChecked();
}

bool options_imp::isNATPMPEnabled() const{
  return checkNATPMP->isChecked();
}

// Return Download & Upload limits in kbps
// [download,upload]
QPair<int,int> options_imp::getGlobalBandwidthLimits() const{
  int DL = -1, UP = -1;
  if(checkDownloadLimit->isChecked()){
    DL = spinDownloadLimit->value();
  }
  if(checkUploadLimit->isChecked()){
    UP = spinUploadLimit->value();
  }
  return qMakePair(DL, UP);
}

bool options_imp::OSDEnabled() const {
  if(checkNoSystray->isChecked()) return false;
  return checkSystrayBalloons->isChecked();
}

bool options_imp::startMinimized() const {
  if(checkStartMinimized->isChecked()) return true;
  return checkStartMinimized->isChecked();
}

bool options_imp::systrayIntegration() const{
  if (!QSystemTrayIcon::isSystemTrayAvailable()) return false;
  return (!checkNoSystray->isChecked());
}

int options_imp::getDHTPort() const {
  if(isDHTPortSameAsBT())
    return 0;
  return spinDHTPort->value();
}

// Return Share ratio
float options_imp::getDesiredRatio() const{
  if(checkRatioLimit->isChecked()){
    return spinRatio->value();
  }
  return -1;
}

// Return Share ratio
float options_imp::getDeleteRatio() const{
  if(checkRatioRemove->isChecked()){
    return spinMaxRatio->value();
  }
  return -1;
}

// Return Save Path
QString options_imp::getSavePath() const{
#ifdef Q_WS_WIN
  QString home = QDir::rootPath();
#else
  QString home = QDir::homePath();
#endif
  if(home[home.length()-1] != QDir::separator()){
    home += QDir::separator();
  }
  if(textSavePath->text().trimmed().isEmpty()){
    textSavePath->setText(home+QString::fromUtf8("qBT_dir"));
  }
  return textSavePath->text();
}

QString options_imp::getTempPath() const {
  return textTempPath->text();
}

bool options_imp::isTempPathEnabled() const {
  return checkTempFolder->isChecked();
}

// Return max connections number
int options_imp::getMaxConnecs() const{
  if(!checkMaxConnecs->isChecked()){
    return -1;
  }else{
    return spinMaxConnec->value();
  }
}

int options_imp::getMaxConnecsPerTorrent() const{
  if(!checkMaxConnecsPerTorrent->isChecked()){
    return -1;
  }else{
    return spinMaxConnecPerTorrent->value();
  }
}

int options_imp::getMaxUploadsPerTorrent() const{
  if(!checkMaxUploadsPerTorrent->isChecked()){
    return -1;
  }else{
    return spinMaxUploadsPerTorrent->value();
  }
}

void options_imp::on_buttonBox_accepted(){
  if(applyButton->isEnabled()){
    saveOptions();
    applyButton->setEnabled(false);
    this->hide();
    emit status_changed();
  }
  saveWindowState();
  accept();
}

void options_imp::applySettings(QAbstractButton* button) {
  if(button == applyButton){
    saveOptions();
    emit status_changed();
  }
}

void options_imp::closeEvent(QCloseEvent *e){
  setAttribute(Qt::WA_DeleteOnClose);
  e->accept();
}

void options_imp::on_buttonBox_rejected(){
  setAttribute(Qt::WA_DeleteOnClose);
  reject();
}

void options_imp::enableDownloadLimit(bool checked){
  if(checked){
    spinDownloadLimit->setEnabled(true);
  }else{
    spinDownloadLimit->setEnabled(false);
  }
}

void options_imp::enableTempPathInput(bool checked){
  if(checked){
    textTempPath->setEnabled(true);
    browseTempDirButton->setEnabled(true);
  }else{
    textTempPath->setEnabled(false);
    browseTempDirButton->setEnabled(false);
  }
}

bool options_imp::useAdditionDialog() const{
  return checkAdditionDialog->isChecked();
}

void options_imp::enableMaxConnecsLimit(bool checked){
  if(checked) {
    spinMaxConnec->setEnabled(true);
  }else{
    spinMaxConnec->setEnabled(false);
  }
}

void options_imp::enableQueueingSystem(bool checked) {
  if(checked) {
    spinMaxActiveDownloads->setEnabled(true);
    spinMaxActiveUploads->setEnabled(true);
    label_max_active_dl->setEnabled(true);
    label_max_active_up->setEnabled(true);
    maxActiveTorrents_lbl->setEnabled(true);
    spinMaxActiveTorrents->setEnabled(true);
  }else{
    spinMaxActiveDownloads->setEnabled(false);
    spinMaxActiveUploads->setEnabled(false);
    label_max_active_dl->setEnabled(false);
    label_max_active_up->setEnabled(false);
    maxActiveTorrents_lbl->setEnabled(false);
    spinMaxActiveTorrents->setEnabled(false);
  }
}

void options_imp::enableMaxConnecsLimitPerTorrent(bool checked){
  if(checked) {
    spinMaxConnecPerTorrent->setEnabled(true);
  }else{
    spinMaxConnecPerTorrent->setEnabled(false);
  }
}

void options_imp::enableSystrayOptions() {
  checkCloseToSystray->setEnabled(true);
  checkMinimizeToSysTray->setEnabled(true);
  checkSystrayBalloons->setEnabled(true);
}

void options_imp::disableSystrayOptions() {
  checkCloseToSystray->setEnabled(false);
  checkMinimizeToSysTray->setEnabled(false);
  checkSystrayBalloons->setEnabled(false);
}

void options_imp::setSystrayOptionsState(bool checked) {
  if(checked) {
    disableSystrayOptions();
  } else {
    enableSystrayOptions();
  }
}

void options_imp::enableMaxUploadsLimitPerTorrent(bool checked){
  if(checked){
    spinMaxUploadsPerTorrent->setEnabled(true);
  }else{
    spinMaxUploadsPerTorrent->setEnabled(false);
  }
}

void options_imp::enableFilter(bool checked){
  if(checked){
    lblFilterPath->setEnabled(true);
    textFilterPath->setEnabled(true);
    browseFilterButton->setEnabled(true);
  }else{
    lblFilterPath->setEnabled(false);
    textFilterPath->setEnabled(false);
    browseFilterButton->setEnabled(false);
  }
}

void options_imp::enableRSS(bool checked) {
  if(checked){
    groupRSSSettings->setEnabled(true);
  }else{
    groupRSSSettings->setEnabled(false);
  }
}

void options_imp::enableUploadLimit(bool checked){
  if(checked){
    spinUploadLimit->setEnabled(true);
  }else{
    spinUploadLimit->setEnabled(false);
  }
}

void options_imp::enableApplyButton(){
  if(!applyButton->isEnabled()){
    applyButton->setEnabled(true);
  }
}

void options_imp::enableShareRatio(bool checked){
  if(checked){
    spinRatio->setEnabled(true);
  }else{
    spinRatio->setEnabled(false);
  }
}

void options_imp::enableDHTPortSettings(bool checked) {
  if(checked){
    spinDHTPort->setEnabled(true);
    dh_port_lbl->setEnabled(true);
  }else{
    spinDHTPort->setEnabled(false);
    dh_port_lbl->setEnabled(false);
  }
}

void options_imp::enableDHTSettings(bool checked) {
  if(checked){
    checkDifferentDHTPort->setEnabled(true);
    enableDHTPortSettings(checkDifferentDHTPort->isChecked());
  }else{
    checkDifferentDHTPort->setEnabled(false);
    enableDHTPortSettings(false);
  }
}


void options_imp::enableDeleteRatio(bool checked){
  if(checked){
    spinMaxRatio->setEnabled(true);
  }else{
    spinMaxRatio->setEnabled(false);
  }
}

void options_imp::enableProxy(int index){
  if(index){
    //enable
    lblProxyIP->setEnabled(true);
    textProxyIP->setEnabled(true);
    lblProxyPort->setEnabled(true);
    spinProxyPort->setEnabled(true);
    checkProxyAuth->setEnabled(true);
    ProxyConnecsBox->setEnabled(true);
  }else{
    //disable
    lblProxyIP->setEnabled(false);
    textProxyIP->setEnabled(false);
    lblProxyPort->setEnabled(false);
    spinProxyPort->setEnabled(false);
    checkProxyAuth->setEnabled(false);
    checkProxyAuth->setEnabled(false);
    ProxyConnecsBox->setEnabled(false);
  }
}

void options_imp::enableProxyHTTP(int index){
  if(index){
    //enable
    lblProxyIP_http->setEnabled(true);
    textProxyIP_http->setEnabled(true);
    lblProxyPort_http->setEnabled(true);
    spinProxyPort_http->setEnabled(true);
    checkProxyAuth_http->setEnabled(true);
  }else{
    //disable
    lblProxyIP_http->setEnabled(false);
    textProxyIP_http->setEnabled(false);
    lblProxyPort_http->setEnabled(false);
    spinProxyPort_http->setEnabled(false);
    checkProxyAuth_http->setEnabled(false);
    checkProxyAuth_http->setEnabled(false);
  }
}

void options_imp::enableProxyAuth(bool checked){
  if(checked){
    lblProxyUsername->setEnabled(true);
    lblProxyPassword->setEnabled(true);
    textProxyUsername->setEnabled(true);
    textProxyPassword->setEnabled(true);
  }else{
    lblProxyUsername->setEnabled(false);
    lblProxyPassword->setEnabled(false);
    textProxyUsername->setEnabled(false);
    textProxyPassword->setEnabled(false);
  }
}

void options_imp::enableProxyAuthHTTP(bool checked){
  if(checked){
    lblProxyUsername_http->setEnabled(true);
    lblProxyPassword_http->setEnabled(true);
    textProxyUsername_http->setEnabled(true);
    textProxyPassword_http->setEnabled(true);
  }else{
    lblProxyUsername_http->setEnabled(false);
    lblProxyPassword_http->setEnabled(false);
    textProxyUsername_http->setEnabled(false);
    textProxyPassword_http->setEnabled(false);
  }
}

void options_imp::enableDirScan(bool checked){
  if(checked){
    textScanDir->setEnabled(true);
    browseScanDirButton->setEnabled(true);
  }else{
    textScanDir->setEnabled(false);
    browseScanDirButton->setEnabled(false);
  }
}

bool options_imp::isSlashScreenDisabled() const {
  return checkNoSplash->isChecked();
}

bool options_imp::speedInTitleBar() const {
  return checkSpeedInTitle->isChecked();
}

bool options_imp::preAllocateAllFiles() const {
  return checkPreallocateAll->isChecked();
}

bool options_imp::addTorrentsInPause() const {
  return checkStartPaused->isChecked();
}

bool options_imp::isDHTPortSameAsBT() const {
  return !checkDifferentDHTPort->isChecked();
}

// Proxy settings
bool options_imp::isProxyEnabled() const{
  return comboProxyType->currentIndex();
}

bool options_imp::isHTTPProxyEnabled() const {
  return comboProxyType_http->currentIndex();
}

bool options_imp::isProxyAuthEnabled() const{
  return checkProxyAuth->isChecked();
}

QString options_imp::getProxyIp() const{
  QString ip = textProxyIP->text();
  ip = ip.trimmed();
  return ip;
}

QString options_imp::getHTTPProxyIp() const{
  QString ip = textProxyIP_http->text();
  ip = ip.trimmed();
  return ip;
}

unsigned short options_imp::getProxyPort() const{
  return spinProxyPort->value();
}

unsigned short options_imp::getHTTPProxyPort() const{
  return spinProxyPort_http->value();
}

QString options_imp::getProxyUsername() const{
  QString username = textProxyUsername->text();
  username = username.trimmed();
  return username;
}

QString options_imp::getHTTPProxyUsername() const{
  QString username = textProxyUsername_http->text();
  username = username.trimmed();
  return username;
}

QString options_imp::getProxyPassword() const{
  QString password = textProxyPassword->text();
  password = password.trimmed();
  return password;
}

QString options_imp::getHTTPProxyPassword() const{
  QString password = textProxyPassword_http->text();
  password = password.trimmed();
  return password;
}

// Locale Settings
QString options_imp::getLocale() const{
  return locales.at(comboI18n->currentIndex());
}

void options_imp::setLocale(QString locale){
  int indexLocales=locales.indexOf(QRegExp(locale));
  if(indexLocales != -1){
    comboI18n->setCurrentIndex(indexLocales);
  }
}

// Return scan dir set in options
QString options_imp::getScanDir() const {
  if(checkScanDir->isChecked()){
    return textScanDir->text().trimmed();
  }else{
    return QString();
  }
}

// Return action on double-click on a downloading torrent set in options
int options_imp::getActionOnDblClOnTorrentDl() const {
  if(actionTorrentDlOnDblClBox->currentIndex()<1)
    return 0;
  return actionTorrentDlOnDblClBox->currentIndex();
}

// Return action on double-click on a finished torrent set in options
int options_imp::getActionOnDblClOnTorrentFn() const {
  if(actionTorrentFnOnDblClBox->currentIndex()<1)
    return 0;
  return actionTorrentFnOnDblClBox->currentIndex();
}

// Display dialog to choose scan dir
void options_imp::on_browseScanDirButton_clicked() {
#ifdef Q_WS_WIN
  QString dir = QFileDialog::getExistingDirectory(this, tr("Choose scan directory"), QDir::rootPath());
#else
  QString dir = QFileDialog::getExistingDirectory(this, tr("Choose scan directory"), QDir::homePath());
#endif
  if(!dir.isNull()){
    textScanDir->setText(dir);
  }
}

void options_imp::on_browseFilterButton_clicked() {
#ifdef Q_WS_WIN
  QString ipfilter = QFileDialog::getOpenFileName(this, tr("Choose an ip filter file"), QDir::rootPath(), tr("Filters")+QString(" (*.dat *.p2p *.p2b)"));
#else
  QString ipfilter = QFileDialog::getOpenFileName(this, tr("Choose an ip filter file"), QDir::homePath(), tr("Filters")+QString(" (*.dat *.p2p *.p2b)"));
#endif
  if(!ipfilter.isNull()){
    textFilterPath->setText(ipfilter);
  }
}

// Display dialog to choose save dir
void options_imp::on_browseSaveDirButton_clicked(){
  QString def_path = QDir::homePath();
  if(!textSavePath->text().isEmpty())
    def_path = textSavePath->text();
  QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a save directory"), def_path);
  if(!dir.isNull()){
    textSavePath->setText(dir);
  }
}

void options_imp::on_browseTempDirButton_clicked(){
  QString def_path = QDir::homePath();
  if(!textTempPath->text().isEmpty())
    def_path = textTempPath->text();
  QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a save directory"), def_path);
  if(!dir.isNull()){
    textTempPath->setText(dir);
  }
}

// Return Filter object to apply to BT session
QString options_imp::getFilter() const{
  return textFilterPath->text();
}

bool options_imp::isToolbarDisplayed() const {
  return checkDisplayToolbar->isChecked();
}

// Web UI

void options_imp::enableWebUi(bool checkBoxValue){
  groupWebUiServer->setEnabled(checkBoxValue);
  groupWebUiAuth->setEnabled(checkBoxValue);
}

bool options_imp::isWebUiEnabled() const
{
  return checkWebUi->isChecked();
}

quint16 options_imp::webUiPort() const
{
  return spinWebUiPort->value();
}

QString options_imp::webUiUsername() const
{
  return textWebUiUsername->text();
}

QString options_imp::webUiPassword() const
{
  return textWebUiPassword->text();
}
