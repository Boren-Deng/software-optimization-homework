#include "DataGenerator.h"
#include "defines.h"

using namespace std;

const array<int, 13> DataGenerator::daysOfMonth{ 0,31,28,31,30,31,30,31,31,30,31,30,31 };

DataGenerator::DataGenerator(int seed)
{

	engine.seed(seed);
	
	/*small batch generate*/
	fillEntry(smallBatch);
	/*medium batch generate*/
	fillEntry(mediumBatch);
	/*large batch generate*/
	fillEntry(largeBatch);
	generateQuery();

	/*scheme2 convert*/
	scheme2DataConvert();

	scheme3DataConvert();
	scheme4DataConvert();
}

void DataGenerator::fillCharArray(char* target, int offset, int count, int dataToFill)
{
	while (count > 0)
	{
		if (dataToFill > 0)
		{
			target[offset + count - 1] = '0' + dataToFill % 10;
			dataToFill /= 10;
		}
		else
			target[offset + count - 1] = '0';
		count--;
	}
}

bool DataGenerator::isLeapYear(int year)
{
	if (year % 400 == 0)
		return true;
	else if (year%100!=0&&year % 4 == 0)
		return true;
	else
		return false;
}

void DataGenerator::fillEntry(std::vector<std::pair<std::string, person>>& target)
{
	for (auto& each : target)
	{
		//id num generate
		int year = yearDistribution(engine), month = monthDistribution(engine), maxDay = (month == 2 && isLeapYear(year)) ? daysOfMonth[month] + 1 : daysOfMonth[month];
		uniform_int_distribution<> dayGenerate(1, maxDay);
		fillCharArray(each.second.id, 0, 6, regionNumber[regionDistribution(engine)]);
		fillCharArray(each.second.id, 6, 4, year);
		fillCharArray(each.second.id, 10, 2, month);
		fillCharArray(each.second.id, 12, 2, dayGenerate(engine));
		fillCharArray(each.second.id, 14, 4, lastFourDigitDistribution(engine));
		each.second.id[18] = '\0';

		//name generate
		each.second.name[0] = upperGenerate(engine);
		each.second.name[8] = upperGenerate(engine);
		each.second.name[14] = upperGenerate(engine);
		for (int i = 1; i <= 7; i++)
			each.second.name[i] = lowerGenerate(engine);
		for (int i = 9; i <= 13; i++)
			each.second.name[i] = lowerGenerate(engine);
		for (int i = 15; i <= 18; i++)
			each.second.name[i] = lowerGenerate(engine);
		each.second.name[19] = '\0';

		//address generate
		for (int i = 0; i <= 58; i++)
			each.second.address[i] = addressGenerate(engine);
		each.second.address[59] = '\0';

		//phone number generate
		fillCharArray(each.second.phone_num, 0, 3, phoneNumHeaderGenerate(engine));
		fillCharArray(each.second.phone_num, 3, 8, phoneNumRestGenerate(engine));
		each.second.phone_num[11] = '\0';

		each.first = each.second.id;
	}
}

void DataGenerator::generateQuery()
{
	char buf[19];
	for (auto& each : query)
	{
		//id num generate
		int year = yearDistribution(engine), month = monthDistribution(engine), maxDay = (month == 2 && isLeapYear(year)) ? daysOfMonth[month] + 1 : daysOfMonth[month];
		uniform_int_distribution<> dayGenerate(1, maxDay);
		fillCharArray(buf, 0, 6, regionDistribution(engine));
		fillCharArray(buf, 6, 4, year);
		fillCharArray(buf, 10, 2, month);
		fillCharArray(buf, 12, 2, dayGenerate(engine));
		fillCharArray(buf, 14, 4, lastFourDigitDistribution(engine));
		buf[18] = '\0';
		each = buf;
	}
}

void DataGenerator::scheme2DataConvert()
{
	for (int i = 0; i < smallBatch.size(); i++)
		smallBatchScheme2[i] = make_pair(smallBatch[i].first, i);
	for (int i = 0; i < mediumBatch.size(); i++)
		mediumBatchScheme2[i] = make_pair(mediumBatch[i].first, i);
	for (int i = 0; i < largeBatch.size(); i++)
		largeBatchScheme2[i] = make_pair(largeBatch[i].first, i);
}

void DataGenerator::scheme3DataConvert()
{
	for (int i = 0; i < smallBatchScheme2.size(); i++)
		smallBatchScheme3[i] = make_pair(stoll(smallBatchScheme2[i].first), i);
	for (int i = 0; i < mediumBatchScheme2.size(); i++)
		mediumBatchScheme3[i] = make_pair(stoll(mediumBatchScheme2[i].first), i);
	for (int i = 0; i < largeBatchScheme2.size(); i++)
		largeBatchScheme3[i] = make_pair(stoll(largeBatchScheme2[i].first), i);
	for (int i = 0; i < query.size(); i++)
		queryForScheme3[i] = stoll(query[i]);
}

void DataGenerator::scheme4DataConvert()
{
	for (int i = 0; i < smallBatchScheme3.size(); i++)
	{
		int date = smallBatchScheme3[i].first % 100000000 / 10000;
		unsigned int id = compressId(smallBatchScheme3[i].first);
		smallBatchScheme4[i] = pair<pair<int,unsigned int>,pair<unsigned int,int>>(make_pair(dayOfYear(date, isLeapYear(date / 10000)),id),make_pair(id,i));
	}
	for (int i = 0; i < mediumBatchScheme3.size(); i++)
	{
		int date = mediumBatchScheme3[i].first % 100000000 / 10000;
		unsigned int id = compressId(mediumBatchScheme3[i].first);
		mediumBatchScheme4[i] = pair<pair<int, unsigned int>, pair<unsigned int, int>>(make_pair(dayOfYear(date, isLeapYear(date / 10000)), id), make_pair(id, i));
	}
	for (int i = 0; i < largeBatchScheme3.size(); i++)
	{
		int date = largeBatchScheme3[i].first % 100000000 / 10000;
		unsigned int id = compressId(largeBatchScheme3[i].first);
		largeBatchScheme4[i] = pair<pair<int, unsigned int>, pair<unsigned int, int>>(make_pair(dayOfYear(date, isLeapYear(date / 10000)), id), make_pair(id, i));
	}
	for (int i = 0; i < queryForScheme3.size(); i++)
	{
		int date = largeBatchScheme3[i].first % 100000000 / 10000;
		queryForScheme4[i] = pair<int, unsigned int>(dayOfYear(date, isLeapYear(date / 10000)), compressId(queryForScheme3[i]));
	}
}

int DataGenerator::dayOfYear(int monthAndDay,bool isLeap)
{
	int month = monthAndDay / 100,day=monthAndDay%100,result=0;
	for (int i = 1; i < month; i++)
		result += daysOfMonth[i];
	if (month > 2)
		result += int(isLeap);
	result += day;
	return result;
}

unsigned int DataGenerator::compressId(long long id)
{
	unsigned int result = 0;
	result += id % 10000;//1-4λ
	result += id / 100000000 % 100 * 10000;//5-6λ
	result += id / 10000000000 % 100 == 19 ? 0 * 1000000 : 1 * 10000000;//7λ
	result += distance(regionNumber.begin(), lower_bound(regionNumber.begin(), regionNumber.end(), id / 1000000000000)) * 10000000;//8-10λ
	return result;
}

const array<int,1000> DataGenerator::regionNumber = { 110101, 110102, 110105, 110106, 110107, 110108, 110109, 110111, 110112, 110113,
		 110114, 110115, 110116, 110117, 110118, 110119, 120101, 120102, 120103, 120104,
		 120105, 120106, 120107, 120108, 120109, 120110, 120111, 120112, 120113, 120114,
		 120115, 120116, 120117, 130101, 130102, 130104, 130105, 130107, 130108, 130109,
		 130110, 130111, 130121, 130123, 130125, 130126, 130127, 130128, 130129, 130130,
		 130131, 130132, 130133, 130181, 130183, 130184, 130185, 130201, 130202, 130203,
		 130204, 130205, 130207, 130208, 130209, 130223, 130224, 130225, 130227, 130229,
		 130230, 130281, 130283, 130301, 130302, 130303, 130304, 130321, 130322, 130323,
		 130324, 130401, 130402, 130403, 130404, 130406, 130407, 130408, 130423, 130424,
		 130425, 130426, 130427, 130428, 130429, 130430, 130431, 130432, 130433, 130434,
		 130435, 130481, 130501, 130502, 130503, 130521, 130522, 130523, 130524, 130525,
		 130526, 130527, 130528, 130529, 130530, 130531, 130532, 130533, 130534, 130535,
		 130581, 130582, 130601, 130602, 130603, 130604, 130621, 130622, 130623, 130624,
		 130625, 130626, 130627, 130628, 130629, 130630, 130631, 130632, 130633, 130634,
		 130635, 130636, 130637, 130638, 130681, 130682, 130683, 130684, 130701, 130702,
		 130703, 130705, 130706, 130721, 130722, 130723, 130724, 130725, 130726, 130727,
		 130728, 130729, 130730, 130731, 130732, 130733, 130801, 130802, 130803, 130804,
		 130821, 130822, 130823, 130824, 130825, 130826, 130827, 130828, 130881, 130901, 130902, 130903, 130921, 130922, 130923,
		 130924, 130925, 130926, 130927, 130928, 130929, 130930, 130981, 130982, 130983,
		 130984, 131001, 131002, 131003, 131022, 131023, 131024, 131025, 131026, 131028,
		 131081, 131082, 131101, 131102, 131103, 131121, 131122, 131123, 131124, 131125,
		 131126, 131127, 131128, 131181, 131182, 140101, 140105, 140106, 140107, 140108,
		 140109, 140110, 140121, 140122, 140123, 140181, 140201, 140202, 140203, 140211,
		 140212, 140221, 140222, 140223, 140224, 140225, 140226, 140227, 140301, 140302,
		 140303, 140311, 140321, 140322, 140401, 140402, 140411, 140421, 140423, 140424,
		 140425, 140426, 140427, 140428, 140429, 140430, 140431, 140481, 140501, 140502,
		 140521, 140522, 140524, 140525, 140581, 140601, 140602, 140603, 140621, 140622,
		 140623, 140624, 140701, 140702, 140721, 140722, 140723, 140724, 140725, 140726,
		 140727, 140728, 140729, 140781, 140801, 140802, 140821, 140822, 140823, 140824,
		 140825, 140826, 140827, 140828, 140829, 140830, 140881, 140882, 140901, 140902,
		 140921, 140922, 140923, 140924, 140925, 140926, 140927, 140928, 140929, 140930,
		 140931, 140932, 140981, 141001, 141002, 141021, 141022, 141023, 141024, 141025,
		 141026, 141027, 141028, 141029, 141030, 141031, 141032, 141033, 141034, 141081,
		 141082, 141101, 141102, 141121, 141122, 141123, 141124, 141125, 141126, 141127,
	141128, 141129, 141130, 141181, 141182, 150101, 150102, 150103, 150104, 150105, 150121,
		 150122, 150123, 150124, 150125, 150201, 150202, 150203, 150204, 150205, 150206,
		 150207, 150221, 150222, 150223, 150301, 150302, 150303, 150304, 150401, 150402,
		 150403, 150404, 150421, 150422, 150423, 150424, 150425, 150426, 150428, 150429,
		 150430, 150501, 150502, 150521, 150522, 150523, 150524, 150525, 150526, 150581,
		 150601, 150602, 150603, 150621, 150622, 150623, 150624, 150625, 150626, 150627,
		 150701, 150702, 150721, 150722, 150723, 150724, 150725, 150726, 150727, 150781,
		 150782, 150783, 150784, 150785, 150801, 150802, 150821, 150822, 150823, 150824,
		 150825, 150826, 150901, 150902, 150921, 150922, 150923, 150924, 150925, 150926,
		 150927, 150928, 150929, 150981, 152101, 152102, 152103, 152104, 152105, 152106,
		 152121, 152122, 152123, 152124, 152125, 152126, 152127, 152128, 152129, 152130,
		 152131, 152201, 152202, 152221, 152222, 152223, 152224, 152301, 152302, 152321,
		 152322, 152323, 152324, 152325, 152326, 152327, 152501, 152502, 152522, 152523,
		 152524, 152525, 152526, 152527, 152528, 152529, 152530, 152531, 152921, 152922,
		 152923, 210101, 210102, 210103, 210104, 210105, 210106, 210111, 210112, 210113,
		 210114, 210122, 210123, 210124, 210181, 210201, 210202, 210203, 210204, 210211,
		 210212, 210213, 210224, 210281, 210282, 210283, 210301, 210302, 210303, 210304,
		 210311, 210321, 210323, 210381,210401, 210402, 210403, 210404, 210411, 210421, 210422, 210423, 210501, 210502, 210503,
		 210504, 210505, 210521, 210522, 210601, 210602, 210603, 210604, 210624, 210681,
		 210682, 210701, 210702, 210703, 210711, 210726, 210727, 210781, 210782, 210801,
		 210802, 210803, 210804, 210811, 210881, 210882, 210901, 210902, 210903, 210904,
		 210905, 210911, 210921, 210922, 211001, 211002, 211003, 211004, 211005, 211011,
		 211021, 211081, 211101, 211102, 211103, 211121, 211122, 211201, 211202, 211204,
		 211221, 211223, 211224, 211281, 211282, 211301, 211302, 211303, 211321, 211322,
		 211324, 211381, 211382, 211401, 211402, 211403, 211404, 211421, 211422, 211481,
		 220101, 220102, 220103, 220104, 220105, 220106, 220112, 220113, 220122, 220182,
		 220183, 220201, 220202, 220203, 220204, 220211, 220221, 220281, 220282, 220283,
		 220284, 220301, 220302, 220303, 220322, 220323, 220381, 220382, 220401, 220402,
		 220403, 220421, 220422, 220501, 220502, 220503, 220521, 220523, 220524, 220581,
		 220582, 220601, 220602, 220605, 220621, 220622, 220623, 220681, 220701, 220702,
		 220721, 220722, 220723, 220724, 220801, 220802, 220821, 220822, 220881, 220882,
		 222401, 222402, 222403, 222404, 222405, 222406, 222424, 222426, 230101, 230102,
		 230103, 230104, 230108, 230109, 230110, 230111, 230112, 230113, 230123, 230124,
		 230125, 230126, 230127, 230128, 230129, 230181, 230182, 230183, 230184, 230201,
		 230202, 230203, 230204, 230205, 230206, 230207, 230208, 230221, 230223, 230224,
	230225, 230227, 230229, 230230, 230231, 230281, 230301, 230302, 230303, 230304, 230305,
		 230306, 230307, 230321, 230381, 230382, 230401, 230402, 230403, 230404, 230405,
		 230406, 230407, 230421, 230422, 230501, 230502, 230503, 230505, 230506, 230521,
		 230522, 230523, 230524, 230601, 230602, 230603, 230604, 230605, 230606, 230621,
		 230622, 230623, 230624, 230701, 230702, 230703, 230704, 230705, 230706, 230707,
		 230708, 230709, 230710, 230711, 230712, 230713, 230714, 230715, 230716, 230722,
		 230781, 230801, 230803, 230804, 230805, 230811, 230822, 230826, 230828, 230833,
		 230881, 230882, 230901, 230902, 230903, 230904, 230921, 231001, 231002, 231003,
		 231004, 231005, 231024, 231025, 231081, 231083, 231084, 231085, 231086, 231101,
		 231102, 231121, 231123, 231124, 231181, 231182, 231201, 231202, 231221, 231222,
		 231223, 231224, 231225, 231226, 231281, 231282, 231283, 232701, 232721, 232722,
		 232723, 310101, 310104, 310105, 310106, 310107, 310109, 310110, 310112, 310113,
		 310114, 310115, 310116, 310117, 310118, 310120, 310151, 310230, 320101, 320102,
		 320104, 320105, 320106, 320111, 320113, 320114, 320115, 320116, 320117, 320118,
		 320205, 320206, 320211, 320213, 320214, 320281, 320282, 320301, 320302, 320303,
		 320305, 320311, 320312, 320321, 320322, 320324, 320381, 320382, 320401, 320402,
		 320404, 320411, 320412, 320413, 320481, 320501, 320505, 320506, 320507, 320508,
		 320509, 320581, 320582, 320583, 320585, 320601, 320602,320611, 320612, 320621, 320623, 320681, 320682, 320684, 320701, 320703, 320706, 320707,
		 320722, 320723, 320724, 320803, 320804, 320812, 320813, 320826, 320830, 320831,
		 320901, 320902, 320903, 320921, 320922, 320923, 320924, 320925, 320981, 320982,
		 321001, 321002, 321003, 321012, 321023, 321081, 321084, 321088, 321101, 321102,
		 321111, 321112, 321181, 321182, 321183, 321201, 321202, 321203, 321204, 321281,
		 321282, 321283, 321284, 321301, 321302, 321311, 321322, 321323, 321324, 330101,
		 330102, 330103, 330104, 330105, 330106, 330108, 330109, 330110, 330111, 330112,
		 330122, 330127, 330182, 330185, 330203, 330205, 330206, 330211, 330212, 330225,
		 330226, 330281, 330282, 330283, 330301, 330302, 330303, 330304, 330322, 330324,
		 330326, 330327, 330328, 330329, 330381, 330382, 330401, 330402, 330411, 330421,
		 330481, 330482, 330483, 330501, 330502, 330503, 330521, 330522, 330523, 330601,
		 330602, 330603, 330604, 330624, 330681, 330683, 330701, 330702, 330703, 330723
};


