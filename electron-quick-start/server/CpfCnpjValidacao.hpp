#ifndef INCLUDE_CPFCNPJ_VALIDATIONS_
#define INCLUDE_CPFCNPJ_VALIDATIONS_

#include <cstring> // strlen

namespace cpfcnpj_validacao
{
    size_t get_cpf_sum(int (cpf)[], size_t& lim, int index = 0)
    {
        if (index == lim)
            return cpf[index] * 2;
        
        return cpf[index] * (lim + 2 - index) + get_cpf_sum(cpf, lim, index + 1);
    }

    size_t get_cnpj_sum(int (cnpj)[], size_t& lim, int index = 0)
    {
        if (index == lim)
            return cnpj[index] * 2;

        return cnpj[index] * (((lim - index) % 8) + 2) + get_cnpj_sum(cnpj, lim, index + 1);
    }

    bool digits_check(int (cpfcnpj)[], const size_t& size)
    {
        for (size_t i = size - 3; i < size - 1; ++i)
        {
            size_t sum = size == 11 ? get_cpf_sum(cpfcnpj, i) : get_cnpj_sum(cpfcnpj, i);
            if ((sum % 11 < 2 && cpfcnpj[i + 1] != 0) || (sum % 11 >= 2 && cpfcnpj[i + 1] != (11 - (sum % 11))))
                return false;
        }

        return true;
    }

    bool validate_cpfcnpj(const char* number)
    {
        const size_t len = strlen(number);

        if (len != 11 && len != 14)
            return false;

        int* cpfcnpj = new int[len];
        for (int i = 0; i < len; ++i)
        {
            cpfcnpj[i] = *(number + i) - '0';
        }

        bool ret = digits_check(cpfcnpj, len);
        delete [] cpfcnpj;

        return ret;
    }
}

#endif //INCLUDE_CPFCNPJ_VALIDATIONS_