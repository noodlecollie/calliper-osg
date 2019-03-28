#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "Core/globalsingleton.h"
#include <QObject>
#include <QHash>
#include <QMutex>
#include <QMutexLocker>

class BaseSettingsCategory;

// Make sure that only one instance of this class ever exists (created via createSingleton()),
// or you're in for a bad time.
class SettingsManager : public QObject,
                        public GlobalSingleton<SettingsManager>
{
    Q_OBJECT
public:
    template<typename T>
    static T* registerSettingsCategory()
    {
        QMutexLocker locker(&m_RegistrationMutex);

        T* category = new T();

        if ( SettingsManager::singleton() )
        {
            SettingsManager::singleton()->addSettingsCategory(category);
        }
        else
        {
            WaitingCategory* container = new WaitingCategory(category);

            WaitingCategory*& head = SettingsManager::m_WaitingCategories;
            container->next = head;
            head = container;
        }

        return category;
    }

    SettingsManager();

private:
    struct WaitingCategory
    {
        WaitingCategory(BaseSettingsCategory* cat)
            : category(cat),
              next(Q_NULLPTR)
        {
        }

        BaseSettingsCategory* category;
        WaitingCategory* next;
    };

    void addWaitingCategories();
    void addSettingsCategory(BaseSettingsCategory* category);

    static QMutex m_RegistrationMutex;
    static WaitingCategory* m_WaitingCategories;

    QHash<QString, BaseSettingsCategory*> m_Categories;
};

#endif // SETTINGSMANAGER_H
