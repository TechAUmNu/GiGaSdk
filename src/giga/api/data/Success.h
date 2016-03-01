/**
 * @author Thomas Guyard <t.guyard@gigatribe.com>
 */

#ifndef GIGA_DATA_SUCCESS_MODEL_H_
#define GIGA_DATA_SUCCESS_MODEL_H_

#include <cpprest/details/basic_types.h>
#include "../../rest/prepoc_manage.h"

namespace giga {
namespace data {

struct Success {
    utility::string_t success = {};
    
    template <class Manager>
    void visit(const Manager& m) {
        GIGA_MANAGE(m, success);
    }
};

} /* namespace data */
} /* namespace giga */
#endif /* GIGA_DATA_SUCCESS_MODEL_H_ */
