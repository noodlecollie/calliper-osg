#include "basesetting.h"

BaseSetting::BaseSetting(quint32 id, const QString &descriptiveName)
    : QObject(Q_NULLPTR),
      m_ID(id),
      m_DescriptiveName(descriptiveName)
{
}

BaseSetting::~BaseSetting()
{
}

quint32 BaseSetting::id() const
{
    return m_ID;
}

QString BaseSetting::descriptiveName() const
{
    return m_DescriptiveName;
}

void BaseSetting::setDescriptiveName(const QString &name)
{
    m_DescriptiveName = name;
}
