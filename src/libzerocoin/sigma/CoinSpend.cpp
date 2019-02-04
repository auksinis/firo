#include "CoinSpend.h"

namespace  sigma {

CoinSpendV3::CoinSpendV3(
        const ParamsV3* p,
        const PrivateCoinV3& coin,
        const std::vector<PublicCoinV3>& anonymity_set,
        uint256 _accumulatorBlockHash) :
        params(p),
        sigmaProof(p),
        denomination(coin.getPublicCoin().getDenomination()),
        coinSerialNumber(coin.getSerialNumber()),
        accumulatorBlockHash(_accumulatorBlockHash)
{
    if (!HasValidSerial()) {
        throw ZerocoinException("Invalid serial # range");
    }
    SigmaPlusProver<Scalar, GroupElement> sigmaProver(params->get_g(), params->get_h(), params->get_n(), params->get_m());
    //compute inverse of g^s
    GroupElement gs = (params->get_g() * coinSerialNumber).inverse();
    std::vector<GroupElement> C_;
    C_.reserve(anonymity_set.size());
    int coinIndex;
    bool indexFound = false;
    for(int j = 0; j < anonymity_set.size(); ++j){
        if(anonymity_set[j] == coin.getPublicCoin()){
            coinIndex = j;
            indexFound = true;
        }

        C_.emplace_back(anonymity_set[j].getValue() + gs);
    }

    if(!indexFound)
        throw ZerocoinException("No such coin in this anonymity set");

    sigmaProver.proof(C_, coinIndex, coin.getRandomness(), sigmaProof);
}

bool CoinSpendV3::Verify(const std::vector<PublicCoinV3>& anonymity_set) const{
    SigmaPlusVerifier<Scalar, GroupElement> sigmaVerifier(params->get_g(), params->get_h(), params->get_n(), params->get_m());
    //compute inverse of g^s
    GroupElement gs = (params->get_g() * coinSerialNumber).inverse();
    std::vector<GroupElement> C_;
    C_.reserve(anonymity_set.size());
    for(int j = 0; j < anonymity_set.size(); ++j)
        C_.emplace_back(anonymity_set[j].getValue() + gs);

    return sigmaVerifier.verify(C_, sigmaProof);
}

const Scalar& CoinSpendV3::getCoinSerialNumber() {
    return this->coinSerialNumber;
}

CoinDenominationV3 CoinSpendV3::getDenomination() const {
    return denomination;
}

int64_t CoinSpendV3::getIntDenomination() const {
    int64_t denom_value;
    DenominationToInteger(this->denomination, denom_value);
    return denom_value;
}

bool CoinSpendV3::HasValidSerial() const {
    return coinSerialNumber.isMember();
}

} //namespace sigma
