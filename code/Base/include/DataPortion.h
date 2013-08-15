/**
 * @file  DataPortion.h
 * @brief  Definition of the Class DataPortion
 * @date  13-sie-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_3F47DABF_3EBE_4bb2_82FB_628B9CD3C449__INCLUDED_)
#define EA_3F47DABF_3EBE_4bb2_82FB_628B9CD3C449__INCLUDED_

#include <QString>

namespace INZ_project {
namespace Base {
class DataPortion
{

public:
    DataPortion();
    virtual ~DataPortion();

    const QString& getIcingaFormated();

};

} //namespace Base
} //namespace INZ_project
#endif // !defined(EA_3F47DABF_3EBE_4bb2_82FB_628B9CD3C449__INCLUDED_)
