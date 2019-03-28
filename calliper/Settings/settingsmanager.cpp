#include "settingsmanager.h"

#include "Settings/basesettingscategory.h"

#include <QtDebug>

SettingsManager::WaitingCategory* SettingsManager::m_WaitingCategories = Q_NULLPTR;

SettingsManager::SettingsManager()
    : QObject(Q_NULLPTR)
{
    QMutexLocker(&m_RegistrationMutex);

    // There should definitely only ever be one instance of this,
    // because of static use of mutexes.
    Q_ASSERT_X(!m_Instance, Q_FUNC_INFO, "Only one SettingsManager instance is allowed!");

    addWaitingCategories();
}

void SettingsManager::addSettingsCategory(BaseSettingsCategory *category)
{
    Q_ASSERT_X(!m_Categories.contains(category->name()), Q_FUNC_INFO, "Category with this name already exists!");

    m_Categories.insert(category->name(), category);
    category->setParent(this);
}

void SettingsManager::addWaitingCategories()
{
    WaitingCategory*& head = SettingsManager::m_WaitingCategories;

    while ( head )
    {
        WaitingCategory* next = head;
        head = head->next;
        addSettingsCategory(next->category);
        delete next;
    }

    m_WaitingCategories = Q_NULLPTR;
}
