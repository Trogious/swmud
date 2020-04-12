#include "SWPazaakCard.h"

SWPazaakCard::SWPazaakCard() :
	type(C_UNKNOWN), sign(S_UNKNOWN), variant(V_FIRST), doubleSigned(false)
{
}

SWPazaakCard::SWPazaakCard(Type type, Sign sign, Variant variant, bool doubleSigned) :
	type(type), sign(sign), variant(variant), doubleSigned(doubleSigned)
{
}

SWString SWPazaakCard::description() const
{
	SWString desc(isSpecial() ? "" : doubleSigned ? "+/-" : sign == S_MINUS ? "-" : "+");

	switch (type)
	{
	case C_1:
		desc.append("1");
		break;

	case C_2:
		desc.append("2");
		break;

	case C_3:
		desc.append("3");
		break;

	case C_4:
		desc.append("4");
		break;

	case C_5:
		desc.append("5");
		break;

	case C_6:
		desc.append("6");
		break;

	case C_2N4:
		desc.append("2&4");
		break;

	case C_3N6:
		desc.append("3&6");
		break;

	case C_D:
		desc.append("D");
		break;

	case C_1T:
		desc.append("1T");
		break;

	case C_7:
		desc.append("7");
		break;

	case C_8:
		desc.append("8");
		break;

	case C_9:
		desc.append("9");
		break;

	case C_10:
		desc.append("10");
		break;

	case C_12: /* +/- 1/2 card */
		desc.append("1/2");
		break;

	case C_0:
		desc.append("0");
		break;

	default:
		desc.append("0");
		break;
	}

	return desc;
}

SWPazaakCard *SWPazaakCard::newInstance(int type, int sign, int variant, int double_signed)
{
	if (isType(type) && isSign(sign) && isVariant(variant))
	{
		return new SWPazaakCard(static_cast<SWPazaakCard::Type> (type), static_cast<SWPazaakCard::Sign> (sign),
				static_cast<SWPazaakCard::Variant> (variant), static_cast<bool> (double_signed));
	}

	return 0;
}

SWPazaakCard *SWPazaakCard::newInstance(const OBJ_DATA *obj)
{
	return obj->item_type == ITEM_PAZAAK_CARD ? newInstance(obj->value[0], obj->value[1], obj->value[2], obj->value[3])
			: 0;
}

