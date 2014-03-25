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
