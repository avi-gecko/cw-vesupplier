#ifndef VESUPPLIER_H
#define VESUPPLIER_H
#include <QString>

class Vesupplier
{
public:
    Vesupplier();
    Vesupplier(QString nameOrganzation
             , QString OGRN
             , QString address
             , QString nameOwner
             , QString phone
             , unsigned int count
             , unsigned double price);

    void setNameOrganization(const QString nameOrganizaton);
    void setOGRN(const QString OGRN);
    void setAddress(const QString address);
    void setNameOwner(const QString nameOwner);
    void setPhone(const QString phone);
    void setCount(const unsigned int count);
    void setPrice(const unsigned double price);

    QString getNameOrganization() const;
    QString getOGRN() const;
    QString getAddress() const;
    QString getNameOwner() const;
    QString getPhone() const;
    unsigned int getCount() const;
    unsigned double getPrice() const;

private:
    Vesupplier(const Vesupplier&) = delete;

    QString m_nameOrganizaton;
    QString m_OGRN;
    QString m_address;
    QString m_nameOwner;
    QString m_phone;
    unsigned int m_count;
    unsigned int m_price;
};

#endif // VESUPPLIER_H
