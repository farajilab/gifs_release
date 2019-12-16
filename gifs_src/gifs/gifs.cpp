#include "gifs.hpp"
#include <vector>
#include "properties.hpp"


void create_qm_interface(size_t nqm, int* qm_atomids)
{
    if (qm == nullptr) {
        std::vector<int> qmids(qm_atomids, qm_atomids+nqm);
        qm = new QMInterface(nqm, qmids);
    };
}


float gifs_get_forces(float* qm_crd, size_t nmm, float* mm_crd, float* mm_chg, float* f, float* fshift)
{
    //
    size_t nqm = qm->get_nqm();
    qm->update(qm_crd, nmm, mm_crd, mm_chg);
    // 
    std::vector<double> g_qm(nqm*3), g_mm(3*nmm);
    //
    PropMap props{};
    props.emplace(QMProperty::qmgradient, &g_qm);
    props.emplace(QMProperty::mmgradient, &g_mm);
    //
    qm->get_properties(props);
    //
    int i = 0;
    int j = 0;
    //
    for (auto& fqm: g_qm) {
        j = i++;
        f[j] = HARTREE_BOHR2MD*fqm;
        fshift[j] = f[j];
    }
    //
    for (auto& fmm: g_mm) {
        j = i++;
        f[j] = HARTREE_BOHR2MD*fmm;
        fshift[j] = f[j];
    }
    //
    return 0.0;

};
