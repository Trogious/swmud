#ifndef CARD_H_
#define CARD_H_
#include "SWString.h"
#include "../structs/obj_data.h"

class SWPazaakCard
{
	public:
		typedef enum
		{
			C_UNKNOWN,
			C_1,
			C_2,
			C_3,
			C_4,
			C_5,
			C_6,
			C_2N4,
			C_3N6,
			C_D,
			C_1T,
			C_7,
			C_8,
			C_9,
			C_10,
			C_12,
			C_0,
			C_MAX,
		} Type;

		typedef enum
		{
			S_UNKNOWN,
			S_PLUS,
			S_MINUS,
			S_MAX
		} Sign;

		typedef enum
		{
			V_UNKNOWN,
			V_FIRST,
			V_SECOND,
			V_MAX
		} Variant;

		SWPazaakCard();
		SWPazaakCard(Type, Sign = S_PLUS, Variant = V_FIRST, bool = false);

	private:
		Type	type;
		Sign	sign;
		Variant	variant;
		bool	doubleSigned;

	public:
		static bool isType(int type) { return type < C_MAX && type > C_UNKNOWN; }
		static bool isSign(int sign) { return sign < S_MAX && sign > S_UNKNOWN; }
		static bool isVariant(int variant) { return variant < V_MAX && variant > V_UNKNOWN; }
		static SWPazaakCard *newInstance(int type, int sign, int variant, int double_signed);
		static SWPazaakCard *newInstance(const OBJ_DATA *);

		Type getType() const { return type; }
		Sign getSign() const { return sign; }
		Variant getVariant() const { return variant; }
		bool isDoubleSigned() const { return doubleSigned; }
		bool isFlippable() const { return type == C_2N4 || type == C_3N6; }
		bool isChangeable() const { return type == C_12; }
		bool isSpecial() const { return type > C_6 && type < C_1T; }
		SWString description() const;
};

#endif /*CARD_H_*/
