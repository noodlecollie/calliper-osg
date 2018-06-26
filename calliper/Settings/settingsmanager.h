#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "Core/globalsingleton.h"
#include <QObject>
#include <QHash>

class BaseSettingsCategory;

// Note that this is NOT thread-safe! There should only ever be one instance.
class SettingsManager : public QObject,
                        public GlobalSingleton<SettingsManager>
{
    Q_OBJECT
public:
    // Returns a const pointer because there should only be one of each type created.
    template<typename T>
    static T* const registerSettingsCategory()
    {
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

    static WaitingCategory* m_WaitingCategories;

    QHash<QString, BaseSettingsCategory*> m_Categories;
};

#endif // SETTINGSMANAGER_H
