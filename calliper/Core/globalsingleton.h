#ifndef GLOBALSINGLETON_H
#define GLOBALSINGLETON_H

#include <QtGlobal>

template<typename T>
class GlobalSingleton
{
public:
    template<typename... ARGS>
    static void createSingleton(ARGS... args)
    {
        if ( m_Instance )
        {
            return;
        }

        m_Instance = new T(std::move(args)...);
    }

    static void destroySingleton()
    {
        if ( !m_Instance )
        {
            return;
        }

        delete m_Instance;
        m_Instance = Q_NULLPTR;
    }

    static T* singleton()
    {
        return m_Instance;
    }

private:
    static T* m_Instance;
};

template<typename T>
T* GlobalSingleton<T>::m_Instance = Q_NULLPTR;

#endif // GLOBALSINGLETON_H
