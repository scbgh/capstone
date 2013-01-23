//
// hasproperties.h
// Support for classes that are editable using the PropertyBrowser widget
//

#include <QtCore>
#include <functional>

#ifndef _HASPROPERTIES_H_
#define _HASPROPERTIES_H_

//
// Holds information about how to read and write a property value
struct PropInfo {
    std::function<QVariant()> queryFunc;
    std::function<void(QVariant)> writeBackFunc;
};

//
// Classes which derive from this has properties and can be edited using the PropertyBrowser widget
class HasProperties {
public:
    const QStringList& properties() { return properties_; }
    QVariant getProperty(const QString& key) { return property_values[key].queryFunc(); }
    void setProperty(const QString& key, const QVariant& val) { property_values[key].writeBackFunc(val); }
protected:
    template <typename T, typename U>
    struct PropertyRegister {
        PropertyRegister(U *ptr, T U::* member, const QString& name)
        {
            auto queryFunc = [=]() { return QVariant::fromValue(ptr->*member); };
            auto writeBackFunc = [=](QVariant val) { ptr->*member = val.value<T>(); };
            ptr->properties_ << name;
            ptr->property_values[name] = {queryFunc, writeBackFunc};
        }
    };
    QMap<QString, PropInfo> property_values;
    QStringList properties_;
};

#define PROP_NAME(name) name ## prop_
#define PROPERTY(cls, ty, name) \
    ty name; \
    HasProperties::PropertyRegister<ty, cls> PROP_NAME(name) {this, &cls::name, #name};

#endif
