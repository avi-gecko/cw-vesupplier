#include "vesupplier.h"

Vesupplier::~Vesupplier()
{

}

Vesupplier::Vesupplier()
{

}

Vesupplier::Vesupplier(Vesupplier *&copy) : m_nameOrganizaton(copy->getNameOrganization())
                                          , m_OGRN(copy->getOGRN())
                                          , m_address(copy->getAddress())
                                          , m_nameOwner(copy->getNameOwner())
                                          , m_phone(copy->getPhone())
                                          , m_countProduct(copy->getCountProduct())
                                          , m_countPost(copy->getCountPost())
                                          , m_price(copy->getPrice())
{

}

Vesupplier::Vesupplier(QString nameOrganzation
                     , QString OGRN
                     , QString address
                     , QString nameOwner
                     , QString phone
                     , unsigned int countProduct
                     , unsigned int countPost
                     , double price)

                     : m_nameOrganizaton(nameOrganzation)
                     , m_OGRN(OGRN)
                     , m_address(address)
                     , m_nameOwner(nameOwner)
                     , m_phone(phone)
                     , m_countProduct(countProduct)
                     , m_countPost(countPost)
                     , m_price(price)
{

}

void Vesupplier::setNameOrganization(const QString nameOrganizaton)
{
    m_nameOrganizaton = nameOrganizaton;
}

void Vesupplier::setOGRN(const QString OGRN)
{
    m_OGRN = OGRN;
}

void Vesupplier::setAddress(const QString address)
{
    m_address = address;
}

void Vesupplier::setNameOwner(const QString nameOwner)
{
    m_nameOwner = nameOwner;
}

void Vesupplier::setPhone(const QString phone)
{
    m_phone = phone;
}

void Vesupplier::setCountProduct(const int countProduct)
{
    m_countProduct = countProduct;
}

void Vesupplier::setCountPost(const int countPost)
{
    m_countPost = countPost;
}

void Vesupplier::setPrice(const double price)
{
    m_price = price;
}

QString Vesupplier::getNameOrganization() const
{
    return m_nameOrganizaton;
}

QString Vesupplier::getOGRN() const
{
    return m_OGRN;
}

QString Vesupplier::getAddress() const
{
    return m_address;
}

QString Vesupplier::getNameOwner() const
{
    return m_nameOwner;
}

QString Vesupplier::getPhone() const
{
    return m_phone;
}

unsigned int Vesupplier::getCountProduct() const
{
    return m_countProduct;
}

unsigned int Vesupplier::getCountPost() const
{
    return m_countPost;
}

double Vesupplier::getPrice() const
{
    return m_price;
}
