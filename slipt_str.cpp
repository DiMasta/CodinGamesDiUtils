vector<string> split(string str, const string& delimiter) {
	vector<string> strs;

	size_t pos = 0;
	string token;
	while ((pos = str.find(delimiter)) != string::npos) {
		token = str.substr(0, pos);
		strs.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	strs.push_back(str);

	return strs;
}
