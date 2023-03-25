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
             , unsigned int countProduct
             , unsigned int countPost
             , double price);

    void setNameOrganization(const QString nameOrganizaton);
    void setOGRN(const QString OGRN);
    void setAddress(const QString address);
    void setNameOwner(const QString nameOwner);
    void setPhone(const QString phone);
    void setCountProduct(const unsigned int countProduct);
    void setCountPost(const unsigned int countProduct);
    void setPrice(const double price);

    QString getNameOrganization() const;
    QString getOGRN() const;
    QString getAddress() const;
    QString getNameOwner() const;
    QString getPhone() const;
    unsigned int getCountProduct() const;
    unsigned int getCountPost() const;
    double getPrice() const;

private:
    QString m_nameOrganizaton;
    QString m_OGRN;
    QString m_address;
    QString m_nameOwner;
    QString m_phone;
    unsigned int m_countProduct;
    unsigned int m_countPost;
    unsigned int m_price;
};

#endif // VESUPPLIER_H
