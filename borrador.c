char * regexString = "\\w\\(x\\)";
	size_t maxGroups = 9;

	regex_t regexCompiled;
	regmatch_t groupArray[maxGroups];

	if (regcomp(&regexCompiled, regexString, REG_EXTENDED)) {
		printf("Could not compile regular expression.\n");
		sthread_exit(-1);
	};

	if (regexec(&regexCompiled, valor, maxGroups, groupArray, 0) == 0) {
		unsigned int g = 0;
		for (g = 0; g < maxGroups; g++)
		{
				if (groupArray[g].rm_so == (size_t)-1)
	            break;  // No more groups
	        char sourceCopy[strlen(valor) + 1];
	        strcpy(sourceCopy, valor);
	        sourceCopy[groupArray[g].rm_eo] = 0;
	        //printf("Group %u: [%2u-%2u]: %s\n",
	        //	g, groupArray[g].rm_so, groupArray[g].rm_eo,
	        //	sourceCopy + groupArray[g].rm_so);

	        // Por cada funcion adicional que encontremos creamos otro grupo
	        argumentos arg;
	    }
	}

	regfree(&regexCompiled);