#include "vesupplier.h"

Vesupplier::Vesupplier()
{

}

Vesupplier::Vesupplier(QString nameOrganzation
                     , QString OGRN
                     , QString address
                     , QString nameOwner
                     , QString phone
                     , unsigned int count
                     , unsigned double price)

                     : m_nameOrganizaton(nameOrganzation)
                     , m_OGRN(OGRN)
                     , m_address(address)
                     , m_nameOwner(nameOwner)
                     , m_phone(phone)
                     , m_count(count)
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

void Vesupplier::setCount(const unsigned int count)
{
    m_count = count;
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

unsigned int Vesupplier::getCount() const
{
    return m_count;
}

double Vesupplier::getPrice() const
{
    return m_price;
}
