#define JSON_TYPE_ARRAY_END ']'
#define JSON_TYPE_OBJECT_END '}'
#define JSON_TYPE_SEPERATOR ','

void ReportJsonLocation(struct jsonparse_state *jsonStatePtr)
{
	msSystem.slogln("at position: ");
	msSystem.slogln(jsonStatePtr->pos);
	msSystem.slogln(" of text: ");
	msSystem.slogln(jsonStatePtr->json);
}

bool AssertParseNext(struct jsonparse_state *jsonStatePtr,
					 byte expectedType)
{
	byte type;

	if ((type = jsonparse_next(jsonStatePtr) != expectedType)) {
		msSystem.slogln("AssertParseNext failed! Expected: ");
		msSystem.slogln(expectedType);
		msSystem.slogln(" but found: ");
		msSystem.slogln(type);
		ReportJsonLocation(jsonStatePtr);
		return false;
	}
	return true;
}

int AssertParseNextMultiple(struct jsonparse_state *jsonStatePtr,
							byte typeA, byte typeB)
{
	byte type = jsonparse_next(jsonStatePtr);

	if (type == typeA)
		return 0;
	if (type == typeB)
		return 1;

	msSystem.slogln("AssertParseNext failed! Expected one of: ");

	msSystem.slogln(typeA);
	msSystem.slogln(" or ");
	msSystem.slogln(typeB);

	msSystem.slogln(" but found: ");
	msSystem.slogln(type);
	ReportJsonLocation(jsonStatePtr);
	return -1;
}

int TryParseKeyValue(struct jsonparse_state *jsonStatePtr, const char *key,
					 char *data, int size)
{
	if (jsonparse_strcmp_value(jsonStatePtr, key) == 0) {
		// 0 means syntax error
		if (!AssertParseNext(jsonStatePtr, JSON_TYPE_PAIR))
			return 0;
		if (!AssertParseNext(jsonStatePtr, JSON_TYPE_STRING))
			return 0;
		jsonparse_copy_value(jsonStatePtr, data, size);

		return 2;				// 2 means correct syntax and key found
	}
	return 1;					// 1 means correct syntax but not found
}

bool ParseAPInfo(struct APInfo * apInfo,
				 struct jsonparse_state * jsonState)
{
	byte type;

	if (!AssertParseNext(jsonState, JSON_TYPE_OBJECT))
		return false;

	while ((type = jsonparse_next(jsonState)) == JSON_TYPE_PAIR_NAME
		   || type == JSON_TYPE_SEPERATOR) {
		if (type == JSON_TYPE_PAIR_NAME) {
			int parseResult;

			if (!
				(parseResult =
				 TryParseKeyValue(jsonState, "ssid", apInfo->ssid,
								  MAX_AP_LEN)))
				return false;
			if (parseResult == 2)
				continue;

			if (!
				(parseResult =
				 TryParseKeyValue(jsonState, "pwd", apInfo->password,
								  MAX_AP_LEN)))
				return false;
			if (parseResult == 2)
				continue;

			// TODO: improve skipping of unnecessary fields
			char key[20];
			jsonparse_copy_value(jsonState, key, 20);
			msSystem.slogln("ignoring unknown key: ");
			msSystem.slogln(key);
			ReportJsonLocation(jsonState);
			if (!AssertParseNext(jsonState, JSON_TYPE_PAIR))
				return false;
			if (!AssertParseNext(jsonState, JSON_TYPE_STRING))
				return false;
		}
	}

	if (type != JSON_TYPE_OBJECT_END) {
		msSystem.slogln("Expected end of object '}' but got: ");
		msSystem.slogln(type);
		ReportJsonLocation(jsonState);
		return false;
	}
	return true;
}
