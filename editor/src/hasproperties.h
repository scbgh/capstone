//
// hasproperties.h
// Support for classes that are editable using the PropertyBrowser widget
//

#include <QtCore>
#include <functional>

#ifndef _HASPROPERTIES_H_
#define _HASPROPERTIES_H_

class HasProperties;

//
// Holds information about how to read and write a property value
struct PropInfo {
    std::function<QVariant()> queryFunc;
    std::function<void(QVariant)> writeBackFunc;
    int type;
};

//
// Classes which derive from this has properties and can be edited using the PropertyBrowser widget
class HasProperties : public QObject {
    Q_OBJECT

public:
    const QStringList& properties() const { return properties_; }
    int propertyType(const QString& key) { return property_values[key].type; }
    QVariant getProperty(const QString& key) const { return property_values[key].queryFunc(); }
    QString getPropertyDisplay(const QString& key) const;
    void setProperty(const QString& key, const QVariant& val) { property_values[key].writeBackFunc(val); }
    void setPropertyDisplay(const QString& key, const QString& val);

    void beginUpdate() { isUpdating_ = true; }
    void endUpdate() { isUpdating_ = false; emit invalidated(); }

signals:
    void invalidated() const;

protected:
    template <typename T, typename U>
    struct PropertyRegister {
        PropertyRegister(U *ptr, T U::* member, const QString& name)
        {
            auto queryFunc = [=]() { return QVariant::fromValue(ptr->*member); };
            auto writeBackFunc = [=](QVariant val) { ptr->*member = val.value<T>(); };
            int type = qMetaTypeId<T>();
            ptr->properties_ << name;
            ptr->property_values[name] = {queryFunc, writeBackFunc, type};
        }
    };
    QMap<QString, PropInfo> property_values;
    QStringList properties_;
    bool isUpdating_;
};

#define PROP_NAME(name) name ## prop_
#define PROPERTY(cls, ty, name) \
    ty name; \
    HasProperties::PropertyRegister<ty, cls> PROP_NAME(name) {this, &cls::name, #name};

#endif
