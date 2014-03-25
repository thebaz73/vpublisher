#include "segmentationmanager.h"
#include <QDir>

#include "configurationmanager.h"

SegmentationManager::SegmentationManager(Type type, QObject *parent) :
    ConfigurableManager(parent),
    m_type(type),
    m_adapter_no(0),
    m_inputFilename(""),
    m_segmentLength(0),
    m_tempDirectory(""),
    m_filenamePrefix(""),
    m_encodingProfile(""),
    m_dtv_device(NULL)
{
    onUpdateConfiguration();
}

SegmentationManager::Type SegmentationManager::type() const
{
    return m_type;
}

int SegmentationManager::adapterNumber() const
{
    return m_adapter_no;
}

void SegmentationManager::setAdapterNumber(int adapterNumber)
{
    m_adapter_no = adapterNumber;
}

QString SegmentationManager::inputFilename() const
{
    return m_inputFilename;
}

void SegmentationManager::setInputFilename(const QString &inputFilename)
{
    m_inputFilename = inputFilename;
}

int SegmentationManager::segmentLength() const
{
    return m_segmentLength;
}

void SegmentationManager::setSegmentLength(int segmentLength)
{
    m_segmentLength = segmentLength;
}

QString SegmentationManager::tempDirectory() const
{
    return m_tempDirectory;
}

void SegmentationManager::setTempDirectory(const QString &tempDirectory)
{
    m_tempDirectory = tempDirectory;
}

QString SegmentationManager::filenamePrefix() const
{
    return m_filenamePrefix;
}

void SegmentationManager::setFilenamePrefix(const QString &filenamePrefix)
{
    m_filenamePrefix = filenamePrefix;
}

QString SegmentationManager::encodingProfile() const
{
    return m_encodingProfile;
}

void SegmentationManager::setEncodingProfile(const QString &encodingProfile)
{
    m_encodingProfile = encodingProfile;
}

void SegmentationManager::setDtvDevice(DTVDevice *dtv_device)
{
    m_dtv_device = dtv_device;
}

QString SegmentationManager::outdir() const
{
    return m_outdir;
}

int SegmentationManager::exitCode() const
{
    return m_exit_code;
}

void SegmentationManager::addPid(PidType type, int pid)
{
    pids.insert(type, pid);
}

void SegmentationManager::onUpdateConfiguration()
{
    m_outdir = ConfigurationManager::instance()->value("hls","tmp_outdir", "/tmp/hls").toString();
    QDir dir(m_outdir);
    if(!dir.exists()) {
        dir.mkpath(m_outdir);
    }
    setTempDirectory(m_outdir);
    setSegmentLength(ConfigurationManager::instance()->value("hls","segment_length", 15).toInt());
    setFilenamePrefix(ConfigurationManager::instance()->value("hls","filename_prefix", "hls_vpub").toString());
    setEncodingProfile(ConfigurationManager::instance()->value("hls","encoding_profile", "128K").toString());
    setInputFilename(ConfigurationManager::instance()->value("hls","input_filename", "/tmp/outputpipe%1.ts").toString().arg(m_adapter_no));
}

void SegmentationManager::onSegmenterExitCode(int exitCode)
{
    m_exit_code = exitCode;
    emit finished();
}

void SegmentationManager::onSegmentIndexChanged(int segmentIndex, double segmentLength)
{
    char *output_filename = (char *)malloc(sizeof(char) * (m_filenamePrefix.length() + 10));
    snprintf(output_filename, m_filenamePrefix.length() + 10, SEGMENT_NAME_PATTERN, qPrintable(m_filenamePrefix), segmentIndex);

    emit segmentChanged(QString::fromLatin1(output_filename), segmentLength);
}
