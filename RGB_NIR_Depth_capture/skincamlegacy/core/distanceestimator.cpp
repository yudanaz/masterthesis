#include "distanceestimator.h"

DistanceEstimator::DistanceEstimator()
{
}

double DistanceEstimator::estimateDistance(double diff12, double diff13, double diff14, double diff23, double diff24, double diff34, double contr12, double contr13, double contr14, double contr23, double contr24, double contr34)
{
this->diff12 = diff12;
this->diff13 = diff13;
this->diff14 = diff14;
this->diff23 = diff23;
this->diff24 = diff24;
this->diff34 = diff34;
this->contr12 = contr12;
this->contr13 = contr13;
this->contr14 = contr14;
this->contr23 = contr23;
this->contr24 = contr24;
this->contr34 = contr34;

if(diff13 <= 0.265)
{
	if(diff12 <= 0.235)
	{
		if(contr13 <= 0.142)
		{
			if(diff14 <= 0.209)
			{
				if(diff14 <= 0.145)
				{
					if(diff34 <= 0.16)
					{
						if(contr34 <= 0.223)
						{
							if(diff23 <= 0.228)
							{
								if(contr14 <= 0.129)
								{
									if(contr14 <= 0.106) return LM1();
									else return LM2();
								}
								else return LM3();
							}
							else
							{
								if(contr13 <= 0.084)
								{
									if(diff23 <= 0.241) return LM4();
									else
									{
										if(diff23 <= 0.247)
										{
											if(contr34 <= 0.182) return LM5();
											else
											{
												if(diff12 <= 0.178) return LM6();
												else return LM7();
											}
										}
										else return LM8();
									}
								}
								else return LM9();
							}
						}
						else
						{
							if(contr34 <= 0.3)
							{
								if(diff12 <= 0.19)
								{
									if(contr23 <= 0.031) return LM10();
									else
									{
										if(diff14 <= 0.132) return LM11();
										else
										{
											if(diff13 <= 0.039) return LM12();
											else
											{
												if(contr12 <= 0.107) return LM13();
												else return LM14();
											}
										}
									}
								}
								else
								{
									if(diff23 <= 0.234)
									{
										if(contr13 <= 0.108)
										{
											if(contr14 <= 0.227)
											{
												if(diff23 <= 0.208)
												{
													if(diff14 <= 0.115)
													{
														if(contr12 <= 0.142) return LM15();
														else return LM16();
													}
													else
													{
														if(contr34 <= 0.235) return LM17();
														else return LM18();
													}
												}
												else
												{
													if(diff13 <= 0.016) return LM19();
													else return LM20();
												}
											}
											else return LM21();
										}
										else
										{
											if(diff12 <= 0.215)
											{
												if(contr13 <= 0.123) return LM22();
												else
												{
													if(contr24 <= 0.265) return LM23();
													else return LM24();
												}
											}
											else
											{
												if(contr12 <= 0.106) return LM25();
												else return LM26();
											}
										}
									}
									else return LM27();
								}
							}
							else
							{
								if(contr23 <= 0.024)
								{
									if(diff12 <= 0.194) return LM28();
									else
									{
										if(diff34 <= 0.115) return LM29();
										else
										{
											if(contr12 <= 0.09)
											{
												if(diff12 <= 0.195) return LM30();
												else return LM31();
											}
											else return LM32();
										}
									}
								}
								else
								{
									if(diff12 <= 0.207)
									{
										if(diff23 <= 0.228) return LM33();
										else
										{
											if(contr12 <= 0.038) return LM34();
											else return LM35();
										}
									}
									else
									{
										if(diff14 <= 0.095) return LM36();
										else
										{
											if(contr24 <= 0.295) return LM37();
											else return LM38();
										}
									}
								}
							}
						}
					}
					else
					{
						if(diff34 <= 0.203)
						{
							if(contr34 <= 0.279)
							{
								if(contr24 <= 0.294)
								{
									if(contr12 <= 0.161) return LM39();
									else return LM40();
								}
								else
								{
									if(contr12 <= 0.145) return LM41();
									else return LM42();
								}
							}
							else
							{
								if(contr13 <= 0.101) return LM43();
								else
								{
									if(diff13 <= 0.08) return LM44();
									else return LM45();
								}
							}
						}
						else return LM46();
					}
				}
				else
				{
					if(diff23 <= 0.224)
					{
						if(contr34 <= 0.282)
						{
							if(contr12 <= 0.148)
							{
								if(diff34 <= 0.113)
								{
									if(contr23 <= 0.102)
									{
										if(diff13 <= 0.102)
										{
											if(contr12 <= 0.088)
											{
												if(contr12 <= 0.075) return LM47();
												else return LM48();
											}
											else return LM49();
										}
										else return LM50();
									}
									else
									{
										if(contr12 <= 0.127) return LM51();
										else return LM52();
									}
								}
								else
								{
									if(contr13 <= 0.055) return LM53();
									else return LM54();
								}
							}
							else return LM55();
						}
						else
						{
							if(diff23 <= 0.198)
							{
								if(diff14 <= 0.178)
								{
									if(diff12 <= 0.167)
									{
										if(diff23 <= 0.184) return LM56();
										else return LM57();
									}
									else return LM58();
								}
								else
								{
									if(diff23 <= 0.177) return LM59();
									else
									{
										if(contr23 <= 0.025)
										{
											if(diff23 <= 0.195) return LM60();
											else return LM61();
										}
										else
										{
											if(diff34 <= 0.111) return LM62();
											else return LM63();
										}
									}
								}
							}
							else
							{
								if(contr12 <= 0.022)
								{
									if(diff14 <= 0.172) return LM64();
									else
									{
										if(contr14 <= 0.292) return LM65();
										else return LM66();
									}
								}
								else
								{
									if(contr24 <= 0.289)
									{
										if(diff12 <= 0.14)
										{
											if(diff23 <= 0.21) return LM67();
											else
											{
												if(contr14 <= 0.211) return LM68();
												else
												{
													if(contr12 <= 0.065) return LM69();
													else return LM70();
												}
											}
										}
										else return LM71();
									}
									else return LM72();
								}
							}
						}
					}
					else
					{
						if(contr23 <= 0.074) return LM73();
						else
						{
							if(contr12 <= 0.165)
							{
								if(diff34 <= 0.067)
								{
									if(diff34 <= 0.057) return LM74();
									else
									{
										if(contr34 <= 0.185) return LM75();
										else return LM76();
									}
								}
								else
								{
									if(contr14 <= 0.267)
									{
										if(diff34 <= 0.073) return LM77();
										else
										{
											if(contr14 <= 0.261) return LM78();
											else return LM79();
										}
									}
									else return LM80();
								}
							}
							else return LM81();
						}
					}
				}
			}
			else
			{
				if(diff23 <= 0.17)
				{
					if(diff12 <= 0.129)
					{
						if(diff34 <= 0.059)
						{
							if(diff14 <= 0.285)
							{
								if(contr12 <= 0.066)
								{
									if(contr23 <= 0.094)
									{
										if(diff14 <= 0.279)
										{
											if(contr34 <= 0.307) return LM82();
											else
											{
												if(contr23 <= 0.015) return LM83();
												else return LM84();
											}
										}
										else
										{
											if(contr13 <= 0.019) return LM85();
											else
											{
												if(contr14 <= 0.237) return LM86();
												else
												{
													if(contr34 <= 0.213) return LM87();
													else
													{
														if(contr13 <= 0.075) return LM88();
														else
														{
															if(diff12 <= 0.09) return LM89();
															else return LM90();
														}
													}
												}
											}
										}
									}
									else
									{
										if(contr13 <= 0.087) return LM91();
										else
										{
											if(diff14 <= 0.28)
											{
												if(diff34 <= 0.053) return LM92();
												else return LM93();
											}
											else
											{
												if(diff14 <= 0.284) return LM94();
												else return LM95();
											}
										}
									}
								}
								else
								{
									if(diff12 <= 0.084)
									{
										if(diff12 <= 0.063)
										{
											if(contr14 <= 0.204)
											{
												if(contr12 <= 0.113) return LM96();
												else return LM97();
											}
											else return LM98();
										}
										else return LM99();
									}
									else return LM100();
								}
							}
							else
							{
								if(contr12 <= 0.064)
								{
									if(diff14 <= 0.29)
									{
										if(diff34 <= 0.044)
										{
											if(contr13 <= 0.079) return LM101();
											else
											{
												if(contr13 <= 0.088)
												{
													if(diff34 <= 0.034) return LM102();
													else return LM103();
												}
												else return LM104();
											}
										}
										else
										{
											if(contr14 <= 0.302)
											{
												if(contr14 <= 0.235)
												{
													if(diff14 <= 0.288) return LM105();
													else return LM106();
												}
												else
												{
													if(contr14 <= 0.253) return LM107();
													else
													{
														if(diff34 <= 0.053) return LM108();
														else return LM109();
													}
												}
											}
											else return LM110();
										}
									}
									else
									{
										if(contr12 <= 0.054)
										{
											if(contr14 <= 0.325)
											{
												if(contr13 <= 0.068) return LM111();
												else
												{
													if(contr24 <= 0.291) return LM112();
													else return LM113();
												}
											}
											else return LM114();
										}
										else return LM115();
									}
								}
								else return LM116();
							}
						}
						else
						{
							if(diff13 <= 0.092)
							{
								if(diff12 <= 0.071)
								{
									if(contr23 <= 0.044) return LM117();
									else return LM118();
								}
								else return LM119();
							}
							else
							{
								if(contr24 <= 0.306)
								{
									if(diff23 <= 0.16)
									{
										if(diff12 <= 0.026) return LM120();
										else
										{
											if(diff23 <= 0.148)
											{
												if(contr23 <= 0.05)
												{
													if(diff14 <= 0.289) return LM121();
													else return LM122();
												}
												else
												{
													if(diff12 <= 0.113) return LM123();
													else return LM124();
												}
											}
											else
											{
												if(diff12 <= 0.053)
												{
													if(contr23 <= 0.019) return LM125();
													else
													{
														if(diff12 <= 0.037) return LM126();
														else return LM127();
													}
												}
												else
												{
													if(contr23 <= 0.039)
													{
														if(diff34 <= 0.071) return LM128();
														else return LM129();
													}
													else return LM130();
												}
											}
										}
									}
									else
									{
										if(diff12 <= 0.055)
										{
											if(contr12 <= 0.083)
											{
												if(contr34 <= 0.33) return LM131();
												else return LM132();
											}
											else
											{
												if(diff34 <= 0.122)
												{
													if(diff34 <= 0.062) return LM133();
													else return LM134();
												}
												else return LM135();
											}
										}
										else return LM136();
									}
								}
								else
								{
									if(diff13 <= 0.204)
									{
										if(diff13 <= 0.128) return LM137();
										else
										{
											if(contr34 <= 0.324) return LM138();
											else return LM139();
										}
									}
									else
									{
										if(contr34 <= 0.316)
										{
											if(contr12 <= 0.064) return LM140();
											else
											{
												if(contr23 <= 0.058) return LM141();
												else return LM142();
											}
										}
										else return LM143();
									}
								}
							}
						}
					}
					else return LM144();
				}
				else
				{
					if(contr23 <= 0.032)
					{
						if(diff23 <= 0.212)
						{
							if(contr12 <= 0.067)
							{
								if(contr12 <= 0.035) return LM145();
								else
								{
									if(diff24 <= 0.255)
									{
										if(contr24 <= 0.291) return LM146();
										else return LM147();
									}
									else return LM148();
								}
							}
							else
							{
								if(diff23 <= 0.196)
								{
									if(diff23 <= 0.185) return LM149();
									else return LM150();
								}
								else return LM151();
							}
						}
						else return LM152();
					}
					else
					{
						if(diff12 <= 0.061)
						{
							if(contr14 <= 0.192)
							{
								if(diff14 <= 0.263)
								{
									if(contr14 <= 0.177) return LM153();
									else
									{
										if(contr12 <= 0.127) return LM154();
										else return LM155();
									}
								}
								else
								{
									if(contr23 <= 0.039)
									{
										if(contr12 <= 0.086) return LM156();
										else return LM157();
									}
									else return LM158();
								}
							}
							else
							{
								if(diff23 <= 0.204)
								{
									if(contr23 <= 0.083)
									{
										if(contr24 <= 0.312) return LM159();
										else return LM160();
									}
									else
									{
										if(contr14 <= 0.253)
										{
											if(diff23 <= 0.188) return LM161();
											else
											{
												if(contr34 <= 0.2) return LM162();
												else
												{
													if(diff12 <= 0.021) return LM163();
													else
													{
														if(contr14 <= 0.245) return LM164();
														else return LM165();
													}
												}
											}
										}
										else
										{
											if(contr23 <= 0.11)
											{
												if(contr12 <= 0.037)
												{
													if(diff14 <= 0.271) return LM166();
													else return LM167();
												}
												else
												{
													if(contr23 <= 0.098) return LM168();
													else return LM169();
												}
											}
											else
											{
												if(contr13 <= 0.057)
												{
													if(diff13 <= 0.205) return LM170();
													else return LM171();
												}
												else return LM172();
											}
										}
									}
								}
								else
								{
									if(contr23 <= 0.11)
									{
										if(contr14 <= 0.271)
										{
											if(contr23 <= 0.076) return LM173();
											else
											{
												if(diff23 <= 0.211) return LM174();
												else return LM175();
											}
										}
										else
										{
											if(contr23 <= 0.096) return LM176();
											else return LM177();
										}
									}
									else
									{
										if(contr34 <= 0.199) return LM178();
										else
										{
											if(diff23 <= 0.213) return LM179();
											else return LM180();
										}
									}
								}
							}
						}
						else
						{
							if(contr13 <= 0.108)
							{
								if(contr34 <= 0.221)
								{
									if(diff34 <= 0.091)
									{
										if(contr12 <= 0.126)
										{
											if(contr12 <= 0.101)
											{
												if(contr34 <= 0.21)
												{
													if(contr23 <= 0.122)
													{
														if(contr34 <= 0.208) return LM181();
														else return LM182();
													}
													else return LM183();
												}
												else
												{
													if(contr12 <= 0.09) return LM184();
													else return LM185();
												}
											}
											else return LM186();
										}
										else return LM187();
									}
									else
									{
										if(contr13 <= 0.02)
										{
											if(contr12 <= 0.103) return LM188();
											else return LM189();
										}
										else return LM190();
									}
								}
								else
								{
									if(diff14 <= 0.226)
									{
										if(diff13 <= 0.151)
										{
											if(contr24 <= 0.319)
											{
												if(contr12 <= 0.099) return LM191();
												else return LM192();
											}
											else
											{
												if(diff14 <= 0.213) return LM193();
												else return LM194();
											}
										}
										else return LM195();
									}
									else return LM196();
								}
							}
							else
							{
								if(contr13 <= 0.14) return LM197();
								else return LM198();
							}
						}
					}
				}
			}
		}
		else
		{
			if(diff24 <= 0.251)
			{
				if(diff14 <= 0.278)
				{
					if(diff24 <= 0.213)
					{
						if(diff34 <= 0.025)
						{
							if(diff14 <= 0.273)
							{
								if(diff14 <= 0.27)
								{
									if(contr23 <= 0.071) return LM199();
									else
									{
										if(contr23 <= 0.096) return LM200();
										else return LM201();
									}
								}
								else return LM202();
							}
							else
							{
								if(contr12 <= 0.175) return LM203();
								else
								{
									if(contr23 <= 0.056) return LM204();
									else return LM205();
								}
							}
						}
						else
						{
							if(contr13 <= 0.223)
							{
								if(diff24 <= 0.179)
								{
									if(contr23 <= 0.069)
									{
										if(diff23 <= 0.178)
										{
											if(contr13 <= 0.2)
											{
												if(diff12 <= 0.109) return LM206();
												else return LM207();
											}
											else return LM208();
										}
										else
										{
											if(contr13 <= 0.203)
											{
												if(contr12 <= 0.17)
												{
													if(diff12 <= 0.009) return LM209();
													else return LM210();
												}
												else
												{
													if(contr14 <= 0.33) return LM211();
													else return LM212();
												}
											}
											else return LM213();
										}
									}
									else
									{
										if(diff34 <= 0.243) return LM214();
										else return LM215();
									}
								}
								else
								{
									if(diff14 <= 0.201)
									{
										if(contr34 <= 0.125)
										{
											if(contr14 <= 0.312)
											{
												if(diff12 <= 0.141) return LM216();
												else return LM217();
											}
											else
											{
												if(diff12 <= 0.112) return LM218();
												else return LM219();
											}
										}
										else return LM220();
									}
									else
									{
										if(contr12 <= 0.133) return LM221();
										else
										{
											if(diff13 <= 0.237) return LM222();
											else return LM223();
										}
									}
								}
							}
							else
							{
								if(contr34 <= 0.078)
								{
									if(contr14 <= 0.309)
									{
										if(contr12 <= 0.147)
										{
											if(diff12 <= 0.063) return LM224();
											else return LM225();
										}
										else
										{
											if(diff13 <= 0.156) return LM226();
											else return LM227();
										}
									}
									else return LM228();
								}
								else
								{
									if(diff12 <= 0.066) return LM229();
									else
									{
										if(contr24 <= 0.202)
										{
											if(diff12 <= 0.071) return LM230();
											else return LM231();
										}
										else return LM232();
									}
								}
							}
						}
					}
					else
					{
						if(contr24 <= 0.174) return LM233();
						else
						{
							if(diff12 <= 0.043)
							{
								if(contr14 <= 0.288)
								{
									if(diff34 <= 0.041)
									{
										if(diff34 <= 0.037)
										{
											if(contr34 <= 0.301)
											{
												if(diff13 <= 0.238)
												{
													if(contr23 <= 0.044) return LM234();
													else return LM235();
												}
												else return LM236();
											}
											else
											{
												if(diff13 <= 0.237) return LM237();
												else return LM238();
											}
										}
										else return LM239();
									}
									else return LM240();
								}
								else
								{
									if(contr13 <= 0.189) return LM241();
									else
									{
										if(contr14 <= 0.298) return LM242();
										else
										{
											if(contr12 <= 0.109) return LM243();
											else return LM244();
										}
									}
								}
							}
							else
							{
								if(diff13 <= 0.241)
								{
									if(diff23 <= 0.13) return LM245();
									else
									{
										if(contr23 <= 0.052)
										{
											if(diff14 <= 0.271) return LM246();
											else return LM247();
										}
										else
										{
											if(contr12 <= 0.131)
											{
												if(contr24 <= 0.236) return LM248();
												else
												{
													if(diff12 <= 0.044) return LM249();
													else return LM250();
												}
											}
											else
											{
												if(diff12 <= 0.058) return LM251();
												else return LM252();
											}
										}
									}
								}
								else
								{
									if(contr13 <= 0.204) return LM253();
									else return LM254();
								}
							}
						}
					}
				}
				else
				{
					if(contr13 <= 0.225)
					{
						if(diff14 <= 0.294)
						{
							if(contr23 <= 0.089)
							{
								if(contr24 <= 0.227) return LM255();
								else
								{
									if(diff34 <= 0.072)
									{
										if(contr23 <= 0.053) return LM256();
										else
										{
											if(diff14 <= 0.292) return LM257();
											else return LM258();
										}
									}
									else
									{
										if(diff34 <= 0.077)
										{
											if(diff12 <= 0.067) return LM259();
											else return LM260();
										}
										else return LM261();
									}
								}
							}
							else
							{
								if(diff24 <= 0.202)
								{
									if(diff13 <= 0.242)
									{
										if(contr24 <= 0.203)
										{
											if(contr13 <= 0.22)
											{
												if(diff14 <= 0.29)
												{
													if(contr12 <= 0.113) return LM262();
													else return LM263();
												}
												else return LM264();
											}
											else return LM265();
										}
										else return LM266();
									}
									else
									{
										if(contr12 <= 0.114) return LM267();
										else return LM268();
									}
								}
								else
								{
									if(contr12 <= 0.096) return LM269();
									else
									{
										if(contr24 <= 0.187) return LM270();
										else return LM271();
									}
								}
							}
						}
						else
						{
							if(diff12 <= 0.138)
							{
								if(diff14 <= 0.297)
								{
									if(diff13 <= 0.244)
									{
										if(contr34 <= 0.102) return LM272();
										else
										{
											if(diff13 <= 0.233) return LM273();
											else return LM274();
										}
									}
									else
									{
										if(diff34 <= 0.05) return LM275();
										else return LM276();
									}
								}
								else
								{
									if(contr14 <= 0.302) return LM277();
									else return LM278();
								}
							}
							else
							{
								if(contr13 <= 0.195) return LM279();
								else
								{
									if(diff12 <= 0.158)
									{
										if(contr13 <= 0.201)
										{
											if(diff12 <= 0.145) return LM280();
											else
											{
												if(diff13 <= 0.251)
												{
													if(contr34 <= 0.102) return LM281();
													else return LM282();
												}
												else return LM283();
											}
										}
										else
										{
											if(diff34 <= 0.043) return LM284();
											else
											{
												if(contr24 <= 0.187) return LM285();
												else
												{
													if(diff14 <= 0.301)
													{
														if(contr24 <= 0.201) return LM286();
														else
														{
															if(diff14 <= 0.3) return LM287();
															else return LM288();
														}
													}
													else return LM289();
												}
											}
										}
									}
									else return LM290();
								}
							}
						}
					}
					else
					{
						if(contr13 <= 0.24)
						{
							if(diff13 <= 0.241)
							{
								if(diff13 <= 0.226)
								{
									if(diff12 <= 0.098) return LM291();
									else return LM292();
								}
								else return LM293();
							}
							else
							{
								if(diff13 <= 0.255)
								{
									if(contr12 <= 0.119) return LM294();
									else return LM295();
								}
								else
								{
									if(contr34 <= 0.065) return LM296();
									else
									{
										if(diff12 <= 0.148) return LM297();
										else return LM298();
									}
								}
							}
						}
						else
						{
							if(diff13 <= 0.166) return LM299();
							else
							{
								if(contr13 <= 0.263) return LM300();
								else return LM301();
							}
						}
					}
				}
			}
			else
			{
				if(contr14 <= 0.296)
				{
					if(diff12 <= 0.154)
					{
						if(diff14 <= 0.236)
						{
							if(diff12 <= 0.108)
							{
								if(contr12 <= 0.148) return LM302();
								else return LM303();
							}
							else
							{
								if(diff12 <= 0.125) return LM304();
								else
								{
									if(contr12 <= 0.151) return LM305();
									else return LM306();
								}
							}
						}
						else
						{
							if(contr12 <= 0.137) return LM307();
							else return LM308();
						}
					}
					else
					{
						if(diff14 <= 0.109)
						{
							if(diff13 <= 0.069) return LM309();
							else return LM310();
						}
						else return LM311();
					}
				}
				else
				{
					if(diff34 <= 0.117)
					{
						if(diff12 <= 0.065)
						{
							if(diff14 <= 0.255)
							{
								if(contr24 <= 0.216)
								{
									if(diff12 <= 0.041)
									{
										if(contr13 <= 0.21)
										{
											if(contr12 <= 0.095) return LM312();
											else
											{
												if(contr13 <= 0.195) return LM313();
												else
												{
													if(contr12 <= 0.106) return LM314();
													else
													{
														if(diff12 <= 0.025) return LM315();
														else return LM316();
													}
												}
											}
										}
										else
										{
											if(contr12 <= 0.118) return LM317();
											else return LM318();
										}
									}
									else
									{
										if(diff34 <= 0.098)
										{
											if(diff23 <= 0.215)
											{
												if(diff23 <= 0.197) return LM319();
												else
												{
													if(diff23 <= 0.214) return LM320();
													else
													{
														if(diff23 <= 0.215) return LM321();
														else return LM322();
													}
												}
											}
											else return LM323();
										}
										else return LM324();
									}
								}
								else return LM325();
							}
							else
							{
								if(contr13 <= 0.18) return LM326();
								else return LM327();
							}
						}
						else
						{
							if(diff14 <= 0.183)
							{
								if(contr24 <= 0.177) return LM328();
								else
								{
									if(contr24 <= 0.197)
									{
										if(diff13 <= 0.061) return LM329();
										else
										{
											if(contr13 <= 0.219)
											{
												if(diff13 <= 0.079) return LM330();
												else return LM331();
											}
											else return LM332();
										}
									}
									else
									{
										if(contr12 <= 0.091) return LM333();
										else return LM334();
									}
								}
							}
							else
							{
								if(diff23 <= 0.219) return LM335();
								else
								{
									if(contr34 <= 0.097)
									{
										if(contr12 <= 0.122) return LM336();
										else return LM337();
									}
									else return LM338();
								}
							}
						}
					}
					else
					{
						if(contr12 <= 0.125)
						{
							if(diff14 <= 0.156)
							{
								if(contr13 <= 0.179) return LM339();
								else
								{
									if(contr34 <= 0.106)
									{
										if(diff34 <= 0.169) return LM340();
										else
										{
											if(diff34 <= 0.199) return LM341();
											else return LM342();
										}
									}
									else
									{
										if(diff14 <= 0.134)
										{
											if(diff14 <= 0.113) return LM343();
											else
											{
												if(contr14 <= 0.309)
												{
													if(diff13 <= 0.055) return LM344();
													else
													{
														if(diff13 <= 0.097) return LM345();
														else return LM346();
													}
												}
												else return LM347();
											}
										}
										else
										{
											if(diff12 <= 0.178) return LM348();
											else return LM349();
										}
									}
								}
							}
							else
							{
								if(diff23 <= 0.146) return LM350();
								else
								{
									if(contr13 <= 0.188) return LM351();
									else
									{
										if(contr13 <= 0.197) return LM352();
										else
										{
											if(diff13 <= 0.07)
											{
												if(contr14 <= 0.305)
												{
													if(diff23 <= 0.186) return LM353();
													else return LM354();
												}
												else return LM355();
											}
											else return LM356();
										}
									}
								}
							}
						}
						else
						{
							if(contr12 <= 0.145)
							{
								if(diff14 <= 0.197)
								{
									if(diff23 <= 0.134)
									{
										if(contr14 <= 0.311) return LM357();
										else
										{
											if(diff14 <= 0.157)
											{
												if(contr13 <= 0.194) return LM358();
												else return LM359();
											}
											else return LM360();
										}
									}
									else return LM361();
								}
								else
								{
									if(diff34 <= 0.198) return LM362();
									else
									{
										if(diff14 <= 0.201) return LM363();
										else return LM364();
									}
								}
							}
							else return LM365();
						}
					}
				}
			}
		}
	}
	else
	{
		if(diff24 <= 0.276)
		{
			if(contr23 <= 0.037)
			{
				if(diff13 <= 0.22)
				{
					if(contr23 <= 0.016)
					{
						if(diff24 <= 0.257)
						{
							if(contr24 <= 0.28)
							{
								if(contr14 <= 0.021) return LM366();
								else return LM367();
							}
							else return LM368();
						}
						else
						{
							if(contr14 <= 0.17)
							{
								if(diff24 <= 0.271)
								{
									if(contr24 <= 0.272) return LM369();
									else
									{
										if(diff12 <= 0.271)
										{
											if(diff34 <= 0.212) return LM370();
											else return LM371();
										}
										else return LM372();
									}
								}
								else
								{
									if(contr24 <= 0.278) return LM373();
									else return LM374();
								}
							}
							else return LM375();
						}
					}
					else
					{
						if(contr34 <= 0.288)
						{
							if(contr13 <= 0.175)
							{
								if(diff14 <= 0.028) return LM376();
								else return LM377();
							}
							else
							{
								if(diff24 <= 0.257)
								{
									if(diff24 <= 0.233) return LM378();
									else
									{
										if(contr13 <= 0.214)
										{
											if(diff24 <= 0.245) return LM379();
											else return LM380();
										}
										else return LM381();
									}
								}
								else return LM382();
							}
						}
						else
						{
							if(contr12 <= 0.112)
							{
								if(contr23 <= 0.028)
								{
									if(contr23 <= 0.023) return LM383();
									else return LM384();
								}
								else return LM385();
							}
							else return LM386();
						}
					}
				}
				else
				{
					if(contr23 <= 0.021)
					{
						if(contr24 <= 0.275)
						{
							if(diff12 <= 0.276)
							{
								if(diff24 <= 0.231)
								{
									if(contr14 <= 0.038) return LM387();
									else
									{
										if(diff24 <= 0.215) return LM388();
										else return LM389();
									}
								}
								else return LM390();
							}
							else return LM391();
						}
						else return LM392();
					}
					else
					{
						if(contr12 <= 0.138)
						{
							if(diff23 <= 0.034)
							{
								if(contr12 <= 0.044) return LM393();
								else
								{
									if(contr12 <= 0.121)
									{
										if(diff12 <= 0.271)
										{
											if(diff12 <= 0.266) return LM394();
											else return LM395();
										}
										else return LM396();
									}
									else return LM397();
								}
							}
							else
							{
								if(contr12 <= 0.121) return LM398();
								else return LM399();
							}
						}
						else return LM400();
					}
				}
			}
			else
			{
				if(contr24 <= 0.226)
				{
					if(contr24 <= 0.18) return LM401();
					else return LM402();
				}
				else
				{
					if(diff13 <= 0.204)
					{
						if(contr24 <= 0.279)
						{
							if(diff23 <= 0.094)
							{
								if(diff23 <= 0.087) return LM403();
								else return LM404();
							}
							else
							{
								if(contr13 <= 0.208)
								{
									if(contr13 <= 0.181)
									{
										if(diff12 <= 0.284)
										{
											if(diff14 <= 0.008) return LM405();
											else return LM406();
										}
										else return LM407();
									}
									else
									{
										if(diff12 <= 0.285) return LM408();
										else
										{
											if(diff12 <= 0.29) return LM409();
											else return LM410();
										}
									}
								}
								else
								{
									if(contr12 <= 0.268) return LM411();
									else return LM412();
								}
							}
						}
						else
						{
							if(contr12 <= 0.153) return LM413();
							else return LM414();
						}
					}
					else
					{
						if(diff24 <= 0.22)
						{
							if(diff24 <= 0.213) return LM415();
							else return LM416();
						}
						else
						{
							if(diff13 <= 0.229)
							{
								if(contr24 <= 0.257) return LM417();
								else
								{
									if(contr12 <= 0.136) return LM418();
									else return LM419();
								}
							}
							else return LM420();
						}
					}
				}
			}
		}
		else
		{
			if(contr14 <= 0.091)
			{
				if(diff14 <= 0.039)
				{
					if(diff14 <= 0.018)
					{
						if(diff13 <= 0.106)
						{
							if(diff23 <= 0.224) return LM421();
							else return LM422();
						}
						else
						{
							if(contr13 <= 0.202)
							{
								if(contr13 <= 0.134)
								{
									if(contr12 <= 0.231) return LM423();
									else return LM424();
								}
								else
								{
									if(contr23 <= 0.067)
									{
										if(contr13 <= 0.16) return LM425();
										else
										{
											if(contr13 <= 0.173) return LM426();
											else return LM427();
										}
									}
									else
									{
										if(diff14 <= 0.006)
										{
											if(contr24 <= 0.269) return LM428();
											else return LM429();
										}
										else return LM430();
									}
								}
							}
							else return LM431();
						}
					}
					else
					{
						if(contr14 <= 0.018) return LM432();
						else
						{
							if(contr23 <= 0.074) return LM433();
							else
							{
								if(contr13 <= 0.099)
								{
									if(diff12 <= 0.278) return LM434();
									else return LM435();
								}
								else return LM436();
							}
						}
					}
				}
				else
				{
					if(diff12 <= 0.251)
					{
						if(diff23 <= 0.223) return LM437();
						else return LM438();
					}
					else
					{
						if(contr14 <= 0.044)
						{
							if(diff12 <= 0.262) return LM439();
							else
							{
								if(diff13 <= 0.024) return LM440();
								else
								{
									if(diff13 <= 0.072) return LM441();
									else return LM442();
								}
							}
						}
						else
						{
							if(contr13 <= 0.088) return LM443();
							else
							{
								if(diff12 <= 0.263)
								{
									if(diff23 <= 0.222)
									{
										if(diff13 <= 0.051) return LM444();
										else return LM445();
									}
									else return LM446();
								}
								else return LM447();
							}
						}
					}
				}
			}
			else
			{
				if(diff14 <= 0.037)
				{
					if(contr24 <= 0.271)
					{
						if(diff23 <= 0.173)
						{
							if(diff23 <= 0.12) return LM448();
							else
							{
								if(contr24 <= 0.265)
								{
									if(contr12 <= 0.113) return LM449();
									else
									{
										if(contr12 <= 0.129) return LM450();
										else return LM451();
									}
								}
								else return LM452();
							}
						}
						else
						{
							if(contr23 <= 0.051)
							{
								if(contr13 <= 0.153) return LM453();
								else return LM454();
							}
							else
							{
								if(diff12 <= 0.297) return LM455();
								else return LM456();
							}
						}
					}
					else
					{
						if(contr34 <= 0.3)
						{
							if(diff14 <= 0.019) return LM457();
							else
							{
								if(diff23 <= 0.186)
								{
									if(diff12 <= 0.262)
									{
										if(contr12 <= 0.142) return LM458();
										else return LM459();
									}
									else return LM460();
								}
								else return LM461();
							}
						}
						else return LM462();
					}
				}
				else
				{
					if(contr23 <= 0.028)
					{
						if(diff14 <= 0.07)
						{
							if(contr24 <= 0.291)
							{
								if(contr24 <= 0.28) return LM463();
								else
								{
									if(contr34 <= 0.278) return LM464();
									else
									{
										if(diff14 <= 0.047) return LM465();
										else
										{
											if(diff34 <= 0.104) return LM466();
											else
											{
												if(diff12 <= 0.242) return LM467();
												else return LM468();
											}
										}
									}
								}
							}
							else
							{
								if(diff13 <= 0.044) return LM469();
								else return LM470();
							}
						}
						else return LM471();
					}
					else
					{
						if(contr34 <= 0.285)
						{
							if(contr12 <= 0.187)
							{
								if(diff14 <= 0.07)
								{
									if(diff13 <= 0.048) return LM472();
									else
									{
										if(contr34 <= 0.265) return LM473();
										else return LM474();
									}
								}
								else
								{
									if(diff23 <= 0.224)
									{
										if(contr12 <= 0.107) return LM475();
										else return LM476();
									}
									else return LM477();
								}
							}
							else
							{
								if(diff23 <= 0.218) return LM478();
								else
								{
									if(contr23 <= 0.143)
									{
										if(diff34 <= 0.095) return LM479();
										else
										{
											if(contr12 <= 0.199) return LM480();
											else return LM481();
										}
									}
									else return LM482();
								}
							}
						}
						else
						{
							if(diff23 <= 0.22)
							{
								if(diff13 <= 0.116)
								{
									if(diff12 <= 0.248)
									{
										if(contr12 <= 0.107) return LM483();
										else
										{
											if(contr12 <= 0.112) return LM484();
											else return LM485();
										}
									}
									else
									{
										if(diff14 <= 0.044)
										{
											if(contr23 <= 0.033) return LM486();
											else return LM487();
										}
										else return LM488();
									}
								}
								else return LM489();
							}
							else
							{
								if(contr12 <= 0.104)
								{
									if(diff23 <= 0.233)
									{
										if(diff12 <= 0.259)
										{
											if(contr12 <= 0.078) return LM490();
											else return LM491();
										}
										else return LM492();
									}
									else return LM493();
								}
								else return LM494();
							}
						}
					}
				}
			}
		}
	}
}
else
{
	if(diff23 <= 0.138)
	{
		if(diff14 <= 0.158)
		{
			if(contr23 <= 0.054)
			{
				if(diff13 <= 0.29)
				{
					if(diff23 <= 0.03)
					{
						if(contr23 <= 0.027) return LM495();
						else
						{
							if(contr23 <= 0.045)
							{
								if(contr12 <= 0.137)
								{
									if(contr12 <= 0.049) return LM496();
									else
									{
										if(diff12 <= 0.272) return LM497();
										else return LM498();
									}
								}
								else
								{
									if(contr14 <= 0.053)
									{
										if(contr14 <= 0.017) return LM499();
										else return LM500();
									}
									else return LM501();
								}
							}
							else return LM502();
						}
					}
					else
					{
						if(contr23 <= 0.032) return LM503();
						else
						{
							if(contr24 <= 0.255)
							{
								if(diff13 <= 0.28) return LM504();
								else
								{
									if(contr14 <= 0.12)
									{
										if(contr14 <= 0.05)
										{
											if(diff13 <= 0.284) return LM505();
											else return LM506();
										}
										else
										{
											if(contr13 <= 0.191)
											{
												if(diff12 <= 0.218) return LM507();
												else return LM508();
											}
											else return LM509();
										}
									}
									else return LM510();
								}
							}
							else return LM511();
						}
					}
				}
				else
				{
					if(contr13 <= 0.193) return LM512();
					else
					{
						if(diff23 <= 0.082) return LM513();
						else
						{
							if(diff23 <= 0.118)
							{
								if(diff13 <= 0.298) return LM514();
								else
								{
									if(contr14 <= 0.071)
									{
										if(diff14 <= 0.125) return LM515();
										else return LM516();
									}
									else return LM517();
								}
							}
							else return LM518();
						}
					}
				}
			}
			else
			{
				if(contr34 <= 0.189)
				{
					if(contr34 <= 0.123)
					{
						if(contr12 <= 0.143) return LM519();
						else return LM520();
					}
					else
					{
						if(diff34 <= 0.174) return LM521();
						else return LM522();
					}
				}
				else
				{
					if(diff13 <= 0.308)
					{
						if(contr12 <= 0.124) return LM523();
						else
						{
							if(contr34 <= 0.25)
							{
								if(diff34 <= 0.185)
								{
									if(diff34 <= 0.177) return LM524();
									else return LM525();
								}
								else return LM526();
							}
							else return LM527();
						}
					}
					else
					{
						if(contr24 <= 0.224)
						{
							if(contr24 <= 0.209)
							{
								if(contr14 <= 0.043) return LM528();
								else return LM529();
							}
							else return LM530();
						}
						else return LM531();
					}
				}
			}
		}
		else
		{
			if(diff13 <= 0.284)
			{
				if(contr14 <= 0.218)
				{
					if(contr12 <= 0.099) return LM532();
					else return LM533();
				}
				else return LM534();
			}
			else
			{
				if(contr23 <= 0.08)
				{
					if(contr34 <= 0.284)
					{
						if(contr13 <= 0.295)
						{
							if(contr34 <= 0.276) return LM535();
							else
							{
								if(contr13 <= 0.218)
								{
									if(contr14 <= 0.068) return LM536();
									else return LM537();
								}
								else return LM538();
							}
						}
						else return LM539();
					}
					else return LM540();
				}
				else
				{
					if(diff23 <= 0.131)
					{
						if(diff23 <= 0.123)
						{
							if(contr23 <= 0.115)
							{
								if(contr23 <= 0.088)
								{
									if(diff23 <= 0.116) return LM541();
									else return LM542();
								}
								else
								{
									if(contr13 <= 0.326)
									{
										if(contr12 <= 0.216) return LM543();
										else return LM544();
									}
									else return LM545();
								}
							}
							else return LM546();
						}
						else
						{
							if(contr12 <= 0.219) return LM547();
							else
							{
								if(diff13 <= 0.331) return LM548();
								else return LM549();
							}
						}
					}
					else
					{
						if(contr12 <= 0.211)
						{
							if(contr12 <= 0.2) return LM550();
							else return LM551();
						}
						else return LM552();
					}
				}
			}
		}
	}
	else
	{
		if(diff14 <= 0.2)
		{
			if(contr34 <= 0.276)
			{
				if(diff23 <= 0.267)
				{
					if(diff24 <= 0.089)
					{
						if(diff14 <= 0.176)
						{
							if(contr12 <= 0.185)
							{
								if(diff34 <= 0.193)
								{
									if(contr34 <= 0.225)
									{
										if(contr12 <= 0.159)
										{
											if(diff23 <= 0.178) return LM553();
											else
											{
												if(diff12 <= 0.137) return LM554();
												else return LM555();
											}
										}
										else
										{
											if(contr14 <= 0.166) return LM556();
											else
											{
												if(diff12 <= 0.156) return LM557();
												else return LM558();
											}
										}
									}
									else
									{
										if(diff23 <= 0.156)
										{
											if(diff34 <= 0.161)
											{
												if(contr23 <= 0.068) return LM559();
												else return LM560();
											}
											else
											{
												if(contr13 <= 0.252) return LM561();
												else
												{
													if(contr34 <= 0.24) return LM562();
													else return LM563();
												}
											}
										}
										else return LM564();
									}
								}
								else
								{
									if(contr12 <= 0.127)
									{
										if(contr14 <= 0.327)
										{
											if(diff12 <= 0.095)
											{
												if(contr13 <= 0.115) return LM565();
												else
												{
													if(contr34 <= 0.146) return LM566();
													else return LM567();
												}
											}
											else
											{
												if(contr14 <= 0.069)
												{
													if(diff34 <= 0.229) return LM568();
													else return LM569();
												}
												else return LM570();
											}
										}
										else
										{
											if(diff12 <= 0.073)
											{
												if(contr12 <= 0.112) return LM571();
												else return LM572();
											}
											else return LM573();
										}
									}
									else
									{
										if(contr13 <= 0.301)
										{
											if(contr13 <= 0.131)
											{
												if(diff12 <= 0.071)
												{
													if(contr12 <= 0.144) return LM574();
													else return LM575();
												}
												else return LM576();
											}
											else
											{
												if(contr13 <= 0.185)
												{
													if(diff14 <= 0.073) return LM577();
													else return LM578();
												}
												else
												{
													if(diff12 <= 0.146)
													{
														if(contr14 <= 0.315)
														{
															if(diff14 <= 0.055) return LM579();
															else return LM580();
														}
														else return LM581();
													}
													else
													{
														if(contr14 <= 0.051) return LM582();
														else
														{
															if(contr12 <= 0.159) return LM583();
															else return LM584();
														}
													}
												}
											}
										}
										else return LM585();
									}
								}
							}
							else
							{
								if(diff14 <= 0.172) return LM586();
								else
								{
									if(contr13 <= 0.278)
									{
										if(diff13 <= 0.328) return LM587();
										else return LM588();
									}
									else return LM589();
								}
							}
						}
						else
						{
							if(contr12 <= 0.223)
							{
								if(contr23 <= 0.167)
								{
									if(contr23 <= 0.099)
									{
										if(contr23 <= 0.087)
										{
											if(contr24 <= 0.195)
											{
												if(contr23 <= 0.07) return LM590();
												else
												{
													if(contr12 <= 0.19) return LM591();
													else
													{
														if(diff12 <= 0.142)
														{
															if(diff34 <= 0.129)
															{
																if(diff34 <= 0.123)
																{
																	if(contr12 <= 0.22) return LM592();
																	else return LM593();
																}
																else return LM594();
															}
															else return LM595();
														}
														else return LM596();
													}
												}
											}
											else
											{
												if(contr14 <= 0.011) return LM597();
												else
												{
													if(diff13 <= 0.33)
													{
														if(contr12 <= 0.177) return LM598();
														else return LM599();
													}
													else
													{
														if(contr12 <= 0.177) return LM600();
														else
														{
															if(contr12 <= 0.183)
															{
																if(contr13 <= 0.244) return LM601();
																else return LM602();
															}
															else return LM603();
														}
													}
												}
											}
										}
										else return LM604();
									}
									else
									{
										if(contr12 <= 0.149)
										{
											if(diff23 <= 0.193)
											{
												if(diff12 <= 0.127) return LM605();
												else return LM606();
											}
											else return LM607();
										}
										else
										{
											if(contr34 <= 0.168) return LM608();
											else
											{
												if(diff13 <= 0.305) return LM609();
												else
												{
													if(diff12 <= 0.111) return LM610();
													else
													{
														if(contr14 <= 0.058) return LM611();
														else return LM612();
													}
												}
											}
										}
									}
								}
								else
								{
									if(contr12 <= 0.135) return LM613();
									else
									{
										if(contr13 <= 0.321)
										{
											if(diff13 <= 0.314)
											{
												if(contr13 <= 0.314) return LM614();
												else return LM615();
											}
											else return LM616();
										}
										else
										{
											if(diff12 <= 0.111) return LM617();
											else return LM618();
										}
									}
								}
							}
							else
							{
								if(contr34 <= 0.188)
								{
									if(contr13 <= 0.303) return LM619();
									else return LM620();
								}
								else
								{
									if(contr13 <= 0.284) return LM621();
									else return LM622();
								}
							}
						}
					}
					else
					{
						if(contr12 <= 0.132)
						{
							if(diff14 <= 0.089) return LM623();
							else
							{
								if(contr23 <= 0.197)
								{
									if(contr34 <= 0.219)
									{
										if(diff14 <= 0.18) return LM624();
										else return LM625();
									}
									else return LM626();
								}
								else return LM627();
							}
						}
						else
						{
							if(contr14 <= 0.311)
							{
								if(contr23 <= 0.157)
								{
									if(contr34 <= 0.211)
									{
										if(contr14 <= 0.308)
										{
											if(contr13 <= 0.316)
											{
												if(diff13 <= 0.289) return LM628();
												else
												{
													if(contr23 <= 0.081) return LM629();
													else return LM630();
												}
											}
											else
											{
												if(diff12 <= 0.088) return LM631();
												else return LM632();
											}
										}
										else return LM633();
									}
									else
									{
										if(contr23 <= 0.08)
										{
											if(diff12 <= 0.092) return LM634();
											else return LM635();
										}
										else
										{
											if(diff12 <= 0.08) return LM636();
											else return LM637();
										}
									}
								}
								else
								{
									if(diff23 <= 0.214) return LM638();
									else return LM639();
								}
							}
							else
							{
								if(contr24 <= 0.181)
								{
									if(contr13 <= 0.191) return LM640();
									else
									{
										if(diff14 <= 0.036) return LM641();
										else return LM642();
									}
								}
								else return LM643();
							}
						}
					}
				}
				else
				{
					if(diff34 <= 0.266)
					{
						if(contr34 <= 0.113)
						{
							if(contr34 <= 0.092)
							{
								if(contr34 <= 0.087) return LM644();
								else return LM645();
							}
							else
							{
								if(contr14 <= 0.313) return LM646();
								else return LM647();
							}
						}
						else
						{
							if(diff34 <= 0.212)
							{
								if(contr14 <= 0.297) return LM648();
								else
								{
									if(contr24 <= 0.154)
									{
										if(contr13 <= 0.058) return LM649();
										else return LM650();
									}
									else
									{
										if(contr13 <= 0.151) return LM651();
										else
										{
											if(contr12 <= 0.145) return LM652();
											else
											{
												if(contr14 <= 0.33) return LM653();
												else return LM654();
											}
										}
									}
								}
							}
							else
							{
								if(diff23 <= 0.305)
								{
									if(contr34 <= 0.231) return LM655();
									else
									{
										if(diff23 <= 0.275) return LM656();
										else return LM657();
									}
								}
								else
								{
									if(contr12 <= 0.172) return LM658();
									else return LM659();
								}
							}
						}
					}
					else
					{
						if(contr14 <= 0.324)
						{
							if(diff34 <= 0.309)
							{
								if(contr12 <= 0.154)
								{
									if(contr13 <= 0.221) return LM660();
									else
									{
										if(diff12 <= 0.02) return LM661();
										else return LM662();
									}
								}
								else return LM663();
							}
							else
							{
								if(diff13 <= 0.305)
								{
									if(contr13 <= 0.187) return LM664();
									else
									{
										if(contr14 <= 0.308) return LM665();
										else return LM666();
									}
								}
								else
								{
									if(diff14 <= 0.007) return LM667();
									else return LM668();
								}
							}
						}
						else
						{
							if(contr12 <= 0.117) return LM669();
							else
							{
								if(contr24 <= 0.17)
								{
									if(contr12 <= 0.171) return LM670();
									else return LM671();
								}
								else return LM672();
							}
						}
					}
				}
			}
			else
			{
				if(contr23 <= 0.056)
				{
					if(diff14 <= 0.173)
					{
						if(diff23 <= 0.201)
						{
							if(contr13 <= 0.167) return LM673();
							else
							{
								if(contr12 <= 0.142)
								{
									if(diff14 <= 0.163)
									{
										if(diff12 <= 0.156) return LM674();
										else return LM675();
									}
									else return LM676();
								}
								else
								{
									if(diff14 <= 0.136) return LM677();
									else
									{
										if(diff14 <= 0.153) return LM678();
										else
										{
											if(diff13 <= 0.33) return LM679();
											else
											{
												if(diff13 <= 0.33) return LM680();
												else return LM681();
											}
										}
									}
								}
							}
						}
						else
						{
							if(diff12 <= 0.069)
							{
								if(diff12 <= 0.055) return LM682();
								else return LM683();
							}
							else
							{
								if(diff23 <= 0.208) return LM684();
								else
								{
									if(contr14 <= 0.123) return LM685();
									else
									{
										if(diff23 <= 0.213) return LM686();
										else return LM687();
									}
								}
							}
						}
					}
					else
					{
						if(contr12 <= 0.127)
						{
							if(contr12 <= 0.068)
							{
								if(contr13 <= 0.102) return LM688();
								else
								{
									if(diff12 <= 0.093) return LM689();
									else return LM690();
								}
							}
							else
							{
								if(diff13 <= 0.302) return LM691();
								else
								{
									if(diff13 <= 0.311) return LM692();
									else
									{
										if(contr13 <= 0.164) return LM693();
										else return LM694();
									}
								}
							}
						}
						else
						{
							if(contr14 <= 0.081)
							{
								if(diff13 <= 0.331) return LM695();
								else
								{
									if(diff14 <= 0.179) return LM696();
									else
									{
										if(contr23 <= 0.051)
										{
											if(diff14 <= 0.185) return LM697();
											else return LM698();
										}
										else
										{
											if(contr12 <= 0.158)
											{
												if(diff14 <= 0.18) return LM699();
												else return LM700();
											}
											else return LM701();
										}
									}
								}
							}
							else return LM702();
						}
					}
				}
				else
				{
					if(diff12 <= 0.091)
					{
						if(diff34 <= 0.106)
						{
							if(diff23 <= 0.221)
							{
								if(diff12 <= 0.083) return LM703();
								else
								{
									if(contr12 <= 0.115)
									{
										if(contr12 <= 0.076) return LM704();
										else
										{
											if(contr12 <= 0.101) return LM705();
											else return LM706();
										}
									}
									else return LM707();
								}
							}
							else return LM708();
						}
						else
						{
							if(contr12 <= 0.068)
							{
								if(diff14 <= 0.103)
								{
									if(contr13 <= 0.175) return LM709();
									else return LM710();
								}
								else
								{
									if(contr23 <= 0.081)
									{
										if(diff12 <= 0.055) return LM711();
										else
										{
											if(diff34 <= 0.138) return LM712();
											else return LM713();
										}
									}
									else
									{
										if(diff13 <= 0.3) return LM714();
										else
										{
											if(diff13 <= 0.316) return LM715();
											else return LM716();
										}
									}
								}
							}
							else
							{
								if(diff12 <= 0.074)
								{
									if(diff13 <= 0.301) return LM717();
									else
									{
										if(contr13 <= 0.091) return LM718();
										else
										{
											if(diff14 <= 0.112)
											{
												if(diff14 <= 0.074) return LM719();
												else return LM720();
											}
											else return LM721();
										}
									}
								}
								else
								{
									if(contr12 <= 0.124)
									{
										if(diff23 <= 0.23)
										{
											if(contr12 <= 0.118)
											{
												if(diff12 <= 0.086) return LM722();
												else
												{
													if(diff13 <= 0.308)
													{
														if(contr12 <= 0.11) return LM723();
														else return LM724();
													}
													else return LM725();
												}
											}
											else
											{
												if(contr13 <= 0.263)
												{
													if(contr12 <= 0.119) return LM726();
													else return LM727();
												}
												else return LM728();
											}
										}
										else
										{
											if(contr23 <= 0.102) return LM729();
											else
											{
												if(diff14 <= 0.108) return LM730();
												else return LM731();
											}
										}
									}
									else
									{
										if(diff14 <= 0.178)
										{
											if(contr24 <= 0.15)
											{
												if(diff23 <= 0.226) return LM732();
												else return LM733();
											}
											else
											{
												if(diff13 <= 0.307)
												{
													if(contr13 <= 0.252) return LM734();
													else return LM735();
												}
												else return LM736();
											}
										}
										else
										{
											if(contr13 <= 0.266) return LM737();
											else return LM738();
										}
									}
								}
							}
						}
					}
					else
					{
						if(diff12 <= 0.154)
						{
							if(diff23 <= 0.188)
							{
								if(diff34 <= 0.14)
								{
									if(contr12 <= 0.095) return LM739();
									else
									{
										if(diff23 <= 0.178)
										{
											if(diff34 <= 0.123) return LM740();
											else return LM741();
										}
										else
										{
											if(contr12 <= 0.126) return LM742();
											else
											{
												if(contr12 <= 0.145) return LM743();
												else return LM744();
											}
										}
									}
								}
								else
								{
									if(contr13 <= 0.218)
									{
										if(contr24 <= 0.234)
										{
											if(contr14 <= 0.114)
											{
												if(diff13 <= 0.321)
												{
													if(contr14 <= 0.074) return LM745();
													else
													{
														if(contr12 <= 0.135)
														{
															if(diff23 <= 0.179) return LM746();
															else
															{
																if(diff34 <= 0.141) return LM747();
																else return LM748();
															}
														}
														else return LM749();
													}
												}
												else
												{
													if(diff14 <= 0.178)
													{
														if(contr24 <= 0.218) return LM750();
														else
														{
															if(contr12 <= 0.117) return LM751();
															else return LM752();
														}
													}
													else return LM753();
												}
											}
											else return LM754();
										}
										else
										{
											if(diff34 <= 0.146)
											{
												if(contr12 <= 0.079) return LM755();
												else return LM756();
											}
											else return LM757();
										}
									}
									else
									{
										if(diff14 <= 0.111) return LM758();
										else
										{
											if(contr23 <= 0.078) return LM759();
											else return LM760();
										}
									}
								}
							}
							else
							{
								if(diff14 <= 0.137)
								{
									if(diff14 <= 0.086)
									{
										if(contr24 <= 0.214) return LM761();
										else
										{
											if(contr14 <= 0.149)
											{
												if(diff12 <= 0.107) return LM762();
												else return LM763();
											}
											else return LM764();
										}
									}
									else
									{
										if(contr14 <= 0.058)
										{
											if(contr34 <= 0.287)
											{
												if(diff13 <= 0.33) return LM765();
												else
												{
													if(contr14 <= 0.015) return LM766();
													else return LM767();
												}
											}
											else return LM768();
										}
										else
										{
											if(diff12 <= 0.123)
											{
												if(contr14 <= 0.072) return LM769();
												else
												{
													if(diff14 <= 0.103) return LM770();
													else
													{
														if(contr13 <= 0.171)
														{
															if(contr24 <= 0.217) return LM771();
															else
															{
																if(diff13 <= 0.326) return LM772();
																else return LM773();
															}
														}
														else
														{
															if(contr13 <= 0.206)
															{
																if(contr12 <= 0.111) return LM774();
																else return LM775();
															}
															else return LM776();
														}
													}
												}
											}
											else
											{
												if(diff12 <= 0.13) return LM777();
												else
												{
													if(diff12 <= 0.133) return LM778();
													else return LM779();
												}
											}
										}
									}
								}
								else
								{
									if(contr12 <= 0.073)
									{
										if(diff14 <= 0.17) return LM780();
										else
										{
											if(diff23 <= 0.199)
											{
												if(contr23 <= 0.067) return LM781();
												else
												{
													if(diff13 <= 0.315) return LM782();
													else return LM783();
												}
											}
											else
											{
												if(contr13 <= 0.131) return LM784();
												else
												{
													if(diff23 <= 0.203) return LM785();
													else return LM786();
												}
											}
										}
									}
									else
									{
										if(contr12 <= 0.122)
										{
											if(diff13 <= 0.32)
											{
												if(diff13 <= 0.31)
												{
													if(diff13 <= 0.303) return LM787();
													else
													{
														if(contr13 <= 0.211)
														{
															if(diff12 <= 0.106) return LM788();
															else
															{
																if(diff13 <= 0.306) return LM789();
																else
																{
																	if(contr13 <= 0.161)
																	{
																		if(contr13 <= 0.152) return LM790();
																		else return LM791();
																	}
																	else return LM792();
																}
															}
														}
														else
														{
															if(diff23 <= 0.21) return LM793();
															else
															{
																if(contr13 <= 0.231) return LM794();
																else
																{
																	if(diff13 <= 0.309) return LM795();
																	else return LM796();
																}
															}
														}
													}
												}
												else
												{
													if(diff23 <= 0.196) return LM797();
													else return LM798();
												}
											}
											else
											{
												if(contr24 <= 0.224)
												{
													if(contr23 <= 0.103)
													{
														if(contr12 <= 0.1) return LM799();
														else
														{
															if(diff23 <= 0.204) return LM800();
															else return LM801();
														}
													}
													else return LM802();
												}
												else return LM803();
											}
										}
										else
										{
											if(contr34 <= 0.287)
											{
												if(contr14 <= 0.013)
												{
													if(diff14 <= 0.158) return LM804();
													else return LM805();
												}
												else
												{
													if(diff12 <= 0.109) return LM806();
													else
													{
														if(diff14 <= 0.186) return LM807();
														else return LM808();
													}
												}
											}
											else
											{
												if(diff13 <= 0.311)
												{
													if(contr13 <= 0.243)
													{
														if(contr12 <= 0.131) return LM809();
														else
														{
															if(contr13 <= 0.237) return LM810();
															else return LM811();
														}
													}
													else
													{
														if(contr23 <= 0.125) return LM812();
														else return LM813();
													}
												}
												else
												{
													if(diff34 <= 0.156) return LM814();
													else return LM815();
												}
											}
										}
									}
								}
							}
						}
						else
						{
							if(diff14 <= 0.166)
							{
								if(diff13 <= 0.325)
								{
									if(diff14 <= 0.101) return LM816();
									else
									{
										if(contr13 <= 0.184)
										{
											if(contr12 <= 0.114) return LM817();
											else
											{
												if(diff23 <= 0.142) return LM818();
												else return LM819();
											}
										}
										else return LM820();
									}
								}
								else
								{
									if(contr12 <= 0.116)
									{
										if(contr12 <= 0.111) return LM821();
										else return LM822();
									}
									else
									{
										if(contr12 <= 0.127) return LM823();
										else
										{
											if(contr12 <= 0.147) return LM824();
											else
											{
												if(contr13 <= 0.224) return LM825();
												else
												{
													if(contr12 <= 0.161)
													{
														if(diff12 <= 0.169)
														{
															if(diff12 <= 0.165) return LM826();
															else return LM827();
														}
														else return LM828();
													}
													else return LM829();
												}
											}
										}
									}
								}
							}
							else
							{
								if(diff12 <= 0.166)
								{
									if(contr12 <= 0.127)
									{
										if(diff14 <= 0.183) return LM830();
										else return LM831();
									}
									else
									{
										if(diff13 <= 0.33)
										{
											if(diff12 <= 0.156) return LM832();
											else return LM833();
										}
										else return LM834();
									}
								}
								else
								{
									if(contr23 <= 0.063) return LM835();
									else
									{
										if(diff14 <= 0.183)
										{
											if(contr24 <= 0.215) return LM836();
											else return LM837();
										}
										else
										{
											if(contr12 <= 0.125) return LM838();
											else return LM839();
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			if(diff23 <= 0.197)
			{
				if(contr12 <= 0.139)
				{
					if(diff12 <= 0.143)
					{
						if(diff12 <= 0.092)
						{
							if(diff23 <= 0.186)
							{
								if(diff14 <= 0.272) return LM840();
								else return LM841();
							}
							else
							{
								if(diff23 <= 0.191) return LM842();
								else
								{
									if(diff13 <= 0.282)
									{
										if(contr13 <= 0.167) return LM843();
										else return LM844();
									}
									else
									{
										if(contr24 <= 0.277) return LM845();
										else return LM846();
									}
								}
							}
						}
						else
						{
							if(contr14 <= 0.185)
							{
								if(contr13 <= 0.163)
								{
									if(diff23 <= 0.173) return LM847();
									else
									{
										if(contr12 <= 0.1)
										{
											if(diff13 <= 0.29) return LM848();
											else
											{
												if(diff23 <= 0.183)
												{
													if(contr12 <= 0.092) return LM849();
													else return LM850();
												}
												else
												{
													if(contr12 <= 0.063) return LM851();
													else
													{
														if(diff14 <= 0.209) return LM852();
														else return LM853();
													}
												}
											}
										}
										else
										{
											if(diff34 <= 0.008) return LM854();
											else return LM855();
										}
									}
								}
								else
								{
									if(diff12 <= 0.11)
									{
										if(diff23 <= 0.187)
										{
											if(diff12 <= 0.101) return LM856();
											else
											{
												if(contr13 <= 0.18)
												{
													if(contr13 <= 0.172) return LM857();
													else return LM858();
												}
												else return LM859();
											}
										}
										else
										{
											if(contr13 <= 0.303)
											{
												if(contr13 <= 0.283) return LM860();
												else return LM861();
											}
											else return LM862();
										}
									}
									else
									{
										if(contr12 <= 0.126)
										{
											if(diff13 <= 0.301) return LM863();
											else
											{
												if(contr12 <= 0.117) return LM864();
												else return LM865();
											}
										}
										else
										{
											if(diff23 <= 0.164)
											{
												if(diff13 <= 0.297) return LM866();
												else
												{
													if(diff13 <= 0.302) return LM867();
													else return LM868();
												}
											}
											else
											{
												if(diff12 <= 0.129)
												{
													if(contr13 <= 0.285) return LM869();
													else return LM870();
												}
												else
												{
													if(contr23 <= 0.048)
													{
														if(contr13 <= 0.17) return LM871();
														else return LM872();
													}
													else
													{
														if(diff14 <= 0.218) return LM873();
														else
														{
															if(diff13 <= 0.303) return LM874();
															else
															{
																if(diff12 <= 0.141) return LM875();
																else return LM876();
															}
														}
													}
												}
											}
										}
									}
								}
							}
							else return LM877();
						}
					}
					else
					{
						if(diff14 <= 0.211)
						{
							if(contr12 <= 0.127)
							{
								if(diff12 <= 0.158)
								{
									if(contr14 <= 0.101) return LM878();
									else return LM879();
								}
								else return LM880();
							}
							else return LM881();
						}
						else
						{
							if(contr12 <= 0.134)
							{
								if(contr12 <= 0.119)
								{
									if(contr12 <= 0.101) return LM882();
									else
									{
										if(diff14 <= 0.221) return LM883();
										else return LM884();
									}
								}
								else
								{
									if(diff23 <= 0.151)
									{
										if(contr13 <= 0.208) return LM885();
										else return LM886();
									}
									else return LM887();
								}
							}
							else return LM888();
						}
					}
				}
				else
				{
					if(diff23 <= 0.18)
					{
						if(diff14 <= 0.217)
						{
							if(contr23 <= 0.085)
							{
								if(contr14 <= 0.074) return LM889();
								else
								{
									if(contr12 <= 0.185) return LM890();
									else
									{
										if(contr13 <= 0.314)
										{
											if(diff23 <= 0.146) return LM891();
											else
											{
												if(contr23 <= 0.062) return LM892();
												else
												{
													if(diff34 <= 0.119) return LM893();
													else return LM894();
												}
											}
										}
										else return LM895();
									}
								}
							}
							else
							{
								if(contr13 <= 0.285)
								{
									if(contr13 <= 0.277) return LM896();
									else return LM897();
								}
								else return LM898();
							}
						}
						else
						{
							if(contr23 <= 0.077)
							{
								if(contr14 <= 0.058)
								{
									if(diff14 <= 0.259)
									{
										if(contr34 <= 0.261)
										{
											if(contr13 <= 0.262) return LM899();
											else
											{
												if(contr12 <= 0.201)
												{
													if(contr13 <= 0.273) return LM900();
													else return LM901();
												}
												else
												{
													if(contr34 <= 0.246) return LM902();
													else return LM903();
												}
											}
										}
										else
										{
											if(contr13 <= 0.235)
											{
												if(diff12 <= 0.128)
												{
													if(diff23 <= 0.177)
													{
														if(diff14 <= 0.256) return LM904();
														else return LM905();
													}
													else return LM906();
												}
												else
												{
													if(diff14 <= 0.24)
													{
														if(diff13 <= 0.309) return LM907();
														else return LM908();
													}
													else return LM909();
												}
											}
											else
											{
												if(contr12 <= 0.203)
												{
													if(diff14 <= 0.242) return LM910();
													else return LM911();
												}
												else return LM912();
											}
										}
									}
									else
									{
										if(diff23 <= 0.171) return LM913();
										else
										{
											if(contr14 <= 0.034)
											{
												if(diff14 <= 0.264) return LM914();
												else return LM915();
											}
											else return LM916();
										}
									}
								}
								else
								{
									if(contr13 <= 0.3)
									{
										if(contr24 <= 0.221)
										{
											if(contr12 <= 0.272)
											{
												if(contr12 <= 0.236)
												{
													if(diff23 <= 0.156) return LM917();
													else
													{
														if(contr12 <= 0.224)
														{
															if(contr13 <= 0.287)
															{
																if(contr12 <= 0.155) return LM918();
																else
																{
																	if(contr23 <= 0.071)
																	{
																		if(contr23 <= 0.065) return LM919();
																		else return LM920();
																	}
																	else return LM921();
																}
															}
															else return LM922();
														}
														else return LM923();
													}
												}
												else
												{
													if(diff23 <= 0.171)
													{
														if(diff13 <= 0.297)
														{
															if(contr13 <= 0.298)
															{
																if(diff13 <= 0.292)
																{
																	if(diff14 <= 0.253) return LM924();
																	else
																	{
																		if(contr14 <= 0.143) return LM925();
																		else return LM926();
																	}
																}
																else return LM927();
															}
															else return LM928();
														}
														else return LM929();
													}
													else
													{
														if(contr13 <= 0.295) return LM930();
														else
														{
															if(diff23 <= 0.178)
															{
																if(contr12 <= 0.256)
																{
																	if(contr34 <= 0.155) return LM931();
																	else
																	{
																		if(diff12 <= 0.11)
																		{
																			if(diff14 <= 0.261) return LM932();
																			else return LM933();
																		}
																		else return LM934();
																	}
																}
																else return LM935();
															}
															else return LM936();
														}
													}
												}
											}
											else
											{
												if(diff13 <= 0.296) return LM937();
												else
												{
													if(diff12 <= 0.16)
													{
														if(diff13 <= 0.298) return LM938();
														else
														{
															if(diff14 <= 0.248) return LM939();
															else return LM940();
														}
													}
													else return LM941();
												}
											}
										}
										else
										{
											if(diff13 <= 0.295)
											{
												if(contr23 <= 0.028)
												{
													if(contr23 <= 0.02) return LM942();
													else
													{
														if(diff23 <= 0.177) return LM943();
														else
														{
															if(diff14 <= 0.263) return LM944();
															else return LM945();
														}
													}
												}
												else
												{
													if(contr23 <= 0.042)
													{
														if(diff12 <= 0.106) return LM946();
														else return LM947();
													}
													else
													{
														if(contr23 <= 0.048) return LM948();
														else
														{
															if(contr12 <= 0.145)
															{
																if(contr12 <= 0.14) return LM949();
																else return LM950();
															}
															else
															{
																if(diff13 <= 0.291) return LM951();
																else return LM952();
															}
														}
													}
												}
											}
											else
											{
												if(contr13 <= 0.21)
												{
													if(diff23 <= 0.158) return LM953();
													else return LM954();
												}
												else return LM955();
											}
										}
									}
									else
									{
										if(contr14 <= 0.167) return LM956();
										else
										{
											if(diff12 <= 0.148) return LM957();
											else return LM958();
										}
									}
								}
							}
							else
							{
								if(diff23 <= 0.173)
								{
									if(diff23 <= 0.152)
									{
										if(contr14 <= 0.041)
										{
											if(diff12 <= 0.165) return LM959();
											else
											{
												if(contr13 <= 0.276) return LM960();
												else return LM961();
											}
										}
										else
										{
											if(diff13 <= 0.319) return LM962();
											else return LM963();
										}
									}
									else
									{
										if(diff23 <= 0.169)
										{
											if(diff14 <= 0.224)
											{
												if(contr12 <= 0.16) return LM964();
												else
												{
													if(contr34 <= 0.142) return LM965();
													else
													{
														if(contr23 <= 0.08)
														{
															if(contr12 <= 0.223) return LM966();
															else return LM967();
														}
														else return LM968();
													}
												}
											}
											else
											{
												if(diff23 <= 0.158) return LM969();
												else
												{
													if(diff13 <= 0.301) return LM970();
													else
													{
														if(contr34 <= 0.127) return LM971();
														else return LM972();
													}
												}
											}
										}
										else
										{
											if(contr12 <= 0.204)
											{
												if(diff34 <= 0.027)
												{
													if(contr13 <= 0.282) return LM973();
													else return LM974();
												}
												else
												{
													if(diff23 <= 0.171)
													{
														if(contr13 <= 0.281) return LM975();
														else return LM976();
													}
													else return LM977();
												}
											}
											else
											{
												if(diff34 <= 0.004) return LM978();
												else
												{
													if(diff14 <= 0.253) return LM979();
													else return LM980();
												}
											}
										}
									}
								}
								else return LM981();
							}
						}
					}
					else
					{
						if(contr24 <= 0.194)
						{
							if(contr12 <= 0.228)
							{
								if(diff13 <= 0.29)
								{
									if(diff23 <= 0.189)
									{
										if(contr12 <= 0.199)
										{
											if(diff12 <= 0.093) return LM982();
											else
											{
												if(contr12 <= 0.162) return LM983();
												else
												{
													if(contr13 <= 0.272) return LM984();
													else return LM985();
												}
											}
										}
										else
										{
											if(contr12 <= 0.216)
											{
												if(contr12 <= 0.203)
												{
													if(contr23 <= 0.076)
													{
														if(diff12 <= 0.091)
														{
															if(diff13 <= 0.272) return LM986();
															else
															{
																if(diff13 <= 0.275) return LM987();
																else return LM988();
															}
														}
														else return LM989();
													}
													else
													{
														if(diff23 <= 0.185) return LM990();
														else return LM991();
													}
												}
												else
												{
													if(diff14 <= 0.254)
													{
														if(contr13 <= 0.278) return LM992();
														else return LM993();
													}
													else
													{
														if(contr13 <= 0.287) return LM994();
														else return LM995();
													}
												}
											}
											else
											{
												if(contr34 <= 0.207) return LM996();
												else
												{
													if(diff12 <= 0.094) return LM997();
													else return LM998();
												}
											}
										}
									}
									else
									{
										if(contr14 <= 0.091)
										{
											if(contr34 <= 0.255)
											{
												if(diff13 <= 0.282)
												{
													if(contr12 <= 0.206)
													{
														if(diff14 <= 0.255) return LM999();
														else return LM1000();
													}
													else
													{
														if(diff13 <= 0.274) return LM1001();
														else
														{
															if(contr13 <= 0.287)
															{
																if(diff13 <= 0.277) return LM1002();
																else return LM1003();
															}
															else return LM1004();
														}
													}
												}
												else
												{
													if(diff13 <= 0.286) return LM1005();
													else
													{
														if(contr12 <= 0.218) return LM1006();
														else return LM1007();
													}
												}
											}
											else
											{
												if(contr23 <= 0.086)
												{
													if(diff14 <= 0.256) return LM1008();
													else return LM1009();
												}
												else
												{
													if(diff13 <= 0.276) return LM1010();
													else return LM1011();
												}
											}
										}
										else
										{
											if(diff23 <= 0.194)
											{
												if(diff14 <= 0.241) return LM1012();
												else return LM1013();
											}
											else return LM1014();
										}
									}
								}
								else
								{
									if(contr34 <= 0.244)
									{
										if(contr34 <= 0.184)
										{
											if(contr23 <= 0.079) return LM1015();
											else
											{
												if(contr12 <= 0.165) return LM1016();
												else
												{
													if(contr12 <= 0.221)
													{
														if(diff23 <= 0.184) return LM1017();
														else return LM1018();
													}
													else return LM1019();
												}
											}
										}
										else
										{
											if(diff12 <= 0.113)
											{
												if(contr34 <= 0.24)
												{
													if(contr13 <= 0.283)
													{
														if(diff13 <= 0.299)
														{
															if(diff13 <= 0.291) return LM1020();
															else return LM1021();
														}
														else return LM1022();
													}
													else
													{
														if(diff23 <= 0.183) return LM1023();
														else
														{
															if(contr13 <= 0.304) return LM1024();
															else return LM1025();
														}
													}
												}
												else return LM1026();
											}
											else
											{
												if(contr13 <= 0.297)
												{
													if(contr34 <= 0.206)
													{
														if(diff12 <= 0.114) return LM1027();
														else return LM1028();
													}
													else
													{
														if(diff13 <= 0.302) return LM1029();
														else
														{
															if(contr13 <= 0.282) return LM1030();
															else return LM1031();
														}
													}
												}
												else return LM1032();
											}
										}
									}
									else
									{
										if(contr13 <= 0.27) return LM1033();
										else
										{
											if(diff12 <= 0.104)
											{
												if(diff12 <= 0.097) return LM1034();
												else return LM1035();
											}
											else return LM1036();
										}
									}
								}
							}
							else
							{
								if(diff14 <= 0.218)
								{
									if(contr13 <= 0.301) return LM1037();
									else
									{
										if(contr13 <= 0.303) return LM1038();
										else return LM1039();
									}
								}
								else
								{
									if(contr12 <= 0.236)
									{
										if(diff14 <= 0.231)
										{
											if(diff12 <= 0.11)
											{
												if(diff23 <= 0.189) return LM1040();
												else return LM1041();
											}
											else return LM1042();
										}
										else return LM1043();
									}
									else return LM1044();
								}
							}
						}
						else
						{
							if(diff12 <= 0.096)
							{
								if(diff12 <= 0.089)
								{
									if(diff23 <= 0.188)
									{
										if(diff14 <= 0.271)
										{
											if(contr13 <= 0.186) return LM1045();
											else return LM1046();
										}
										else return LM1047();
									}
									else
									{
										if(contr24 <= 0.261) return LM1048();
										else return LM1049();
									}
								}
								else
								{
									if(contr12 <= 0.182)
									{
										if(contr23 <= 0.031) return LM1050();
										else return LM1051();
									}
									else
									{
										if(diff13 <= 0.277) return LM1052();
										else
										{
											if(contr23 <= 0.03) return LM1053();
											else return LM1054();
										}
									}
								}
							}
							else
							{
								if(contr24 <= 0.233)
								{
									if(contr13 <= 0.248) return LM1055();
									else
									{
										if(contr12 <= 0.224)
										{
											if(diff13 <= 0.292)
											{
												if(diff14 <= 0.248)
												{
													if(diff13 <= 0.289) return LM1056();
													else return LM1057();
												}
												else return LM1058();
											}
											else return LM1059();
										}
										else
										{
											if(diff13 <= 0.291) return LM1060();
											else
											{
												if(diff23 <= 0.195) return LM1061();
												else return LM1062();
											}
										}
									}
								}
								else
								{
									if(contr23 <= 0.019) return LM1063();
									else
									{
										if(diff12 <= 0.102)
										{
											if(diff23 <= 0.181)
											{
												if(contr24 <= 0.245) return LM1064();
												else return LM1065();
											}
											else
											{
												if(contr13 <= 0.214) return LM1066();
												else
												{
													if(contr12 <= 0.204) return LM1067();
													else return LM1068();
												}
											}
										}
										else return LM1069();
									}
								}
							}
						}
					}
				}
			}
			else
			{
				if(diff23 <= 0.207)
				{
					if(diff13 <= 0.275)
					{
						if(contr23 <= 0.034)
						{
							if(contr12 <= 0.125) return LM1070();
							else return LM1071();
						}
						else
						{
							if(contr13 <= 0.243)
							{
								if(diff13 <= 0.271) return LM1072();
								else
								{
									if(contr23 <= 0.039) return LM1073();
									else
									{
										if(diff13 <= 0.273) return LM1074();
										else return LM1075();
									}
								}
							}
							else return LM1076();
						}
					}
					else
					{
						if(contr34 <= 0.293)
						{
							if(contr23 <= 0.05)
							{
								if(diff13 <= 0.288)
								{
									if(diff13 <= 0.278)
									{
										if(diff12 <= 0.075)
										{
											if(diff12 <= 0.075) return LM1077();
											else return LM1078();
										}
										else return LM1079();
									}
									else return LM1080();
								}
								else return LM1081();
							}
							else
							{
								if(diff13 <= 0.291)
								{
									if(contr13 <= 0.306)
									{
										if(contr23 <= 0.112) return LM1082();
										else
										{
											if(contr14 <= 0.237)
											{
												if(contr12 <= 0.071) return LM1083();
												else
												{
													if(contr14 <= 0.067) return LM1084();
													else return LM1085();
												}
											}
											else return LM1086();
										}
									}
									else
									{
										if(contr23 <= 0.165) return LM1087();
										else
										{
											if(diff12 <= 0.074) return LM1088();
											else return LM1089();
										}
									}
								}
								else
								{
									if(contr14 <= 0.066)
									{
										if(diff13 <= 0.296)
										{
											if(diff14 <= 0.215)
											{
												if(contr14 <= 0.043)
												{
													if(diff13 <= 0.295) return LM1090();
													else return LM1091();
												}
												else return LM1092();
											}
											else return LM1093();
										}
										else
										{
											if(contr14 <= 0.024) return LM1094();
											else return LM1095();
										}
									}
									else
									{
										if(contr12 <= 0.107)
										{
											if(diff14 <= 0.213) return LM1096();
											else
											{
												if(diff12 <= 0.089) return LM1097();
												else return LM1098();
											}
										}
										else return LM1099();
									}
								}
							}
						}
						else
						{
							if(diff13 <= 0.287)
							{
								if(diff23 <= 0.201)
								{
									if(contr24 <= 0.264) return LM1100();
									else return LM1101();
								}
								else
								{
									if(contr24 <= 0.259) return LM1102();
									else return LM1103();
								}
							}
							else
							{
								if(contr12 <= 0.071)
								{
									if(contr12 <= 0.066)
									{
										if(diff12 <= 0.1)
										{
											if(diff14 <= 0.205) return LM1104();
											else return LM1105();
										}
										else return LM1106();
									}
									else return LM1107();
								}
								else
								{
									if(diff13 <= 0.294)
									{
										if(contr14 <= 0.137)
										{
											if(contr12 <= 0.114) return LM1108();
											else return LM1109();
										}
										else return LM1110();
									}
									else
									{
										if(diff23 <= 0.206) return LM1111();
										else return LM1112();
									}
								}
							}
						}
					}
				}
				else
				{
					if(contr34 <= 0.277)
					{
						if(contr24 <= 0.185)
						{
							if(contr13 <= 0.315)
							{
								if(diff14 <= 0.212)
								{
									if(contr12 <= 0.169) return LM1113();
									else return LM1114();
								}
								else return LM1115();
							}
							else
							{
								if(contr14 <= 0.153)
								{
									if(diff12 <= 0.071) return LM1116();
									else
									{
										if(contr12 <= 0.168) return LM1117();
										else return LM1118();
									}
								}
								else return LM1119();
							}
						}
						else
						{
							if(contr23 <= 0.207)
							{
								if(contr34 <= 0.259)
								{
									if(diff12 <= 0.06) return LM1120();
									else return LM1121();
								}
								else
								{
									if(diff14 <= 0.229) return LM1122();
									else return LM1123();
								}
							}
							else return LM1124();
						}
					}
					else
					{
						if(diff13 <= 0.282)
						{
							if(contr23 <= 0.043)
							{
								if(diff13 <= 0.277) return LM1125();
								else return LM1126();
							}
							else return LM1127();
						}
						else
						{
							if(diff12 <= 0.076)
							{
								if(contr23 <= 0.05) return LM1128();
								else
								{
									if(diff12 <= 0.067) return LM1129();
									else
									{
										if(contr13 <= 0.229) return LM1130();
										else
										{
											if(contr13 <= 0.273)
											{
												if(diff14 <= 0.209) return LM1131();
												else return LM1132();
											}
											else return LM1133();
										}
									}
								}
							}
							else
							{
								if(contr23 <= 0.116)
								{
									if(contr13 <= 0.149) return LM1134();
									else return LM1135();
								}
								else
								{
									if(contr12 <= 0.121) return LM1136();
									else
									{
										if(contr13 <= 0.285) return LM1137();
										else return LM1138();
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
return -999999;
}

double DistanceEstimator::LM1()
{
	return -5.6969 * diff12 - 247.0656 * diff13 + 711.6174 * diff14 - 375.3592 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 + 27.1492 * contr12 - 753.8198 * contr13 + 3.2663 * contr14 + 115.4975 * contr23 - 125.6655 * contr24 + 86.9757 * contr34 + 2784.8411 ;
}

double DistanceEstimator::LM2()
{
	return -5.6969 * diff12 - 247.0656 * diff13 + 711.6174 * diff14 - 375.3592 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 + 262.3185 * contr12 - 411.5245 * contr13 + 3.2663 * contr14 + 115.4975 * contr23 - 125.6655 * contr24 + 86.9757 * contr34 + 2717.4788 ;
}

double DistanceEstimator::LM3()
{
	return -5.6969 * diff12 + 582.6364 * diff13 + 1072.4508 * diff14 - 375.3592 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 + 3016.7818 * contr12 - 344.0582 * contr13 + 3.2663 * contr14 + 115.4975 * contr23 - 125.6655 * contr24 + 86.9757 * contr34 + 2160.0417 ;
}

double DistanceEstimator::LM4()
{
	return -1105.9286 * diff12 - 29.9662 * diff13 + 538.3436 * diff14 + 27.5056 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 + 107.9865 * contr12 + 78.5242 * contr13 + 3.2663 * contr14 + 115.4975 * contr23 - 125.6655 * contr24 + 109.004 * contr34 + 2696.9658 ;
}

double DistanceEstimator::LM5()
{
	return -1274.9225 * diff12 - 29.9662 * diff13 - 189.5725 * diff14 - 1829.2004 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 + 107.9865 * contr12 + 78.5242 * contr13 + 3.2663 * contr14 + 115.4975 * contr23 - 125.6655 * contr24 - 749.9356 * contr34 + 3507.8011 ;
}

double DistanceEstimator::LM6()
{
	return -1670.0781 * diff12 - 29.9662 * diff13 - 189.5725 * diff14 - 2322.6331 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 + 107.9865 * contr12 + 78.5242 * contr13 + 3.2663 * contr14 + 115.4975 * contr23 - 125.6655 * contr24 - 1023.2346 * contr34 + 3744.7379 ;
}

double DistanceEstimator::LM7()
{
	return -1687.7477 * diff12 - 29.9662 * diff13 - 189.5725 * diff14 - 1829.2004 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 + 107.9865 * contr12 + 78.5242 * contr13 + 3.2663 * contr14 + 115.4975 * contr23 - 125.6655 * contr24 - 1023.2346 * contr34 + 3624.0378 ;
}

double DistanceEstimator::LM8()
{
	return -919.3015 * diff12 - 29.9662 * diff13 + 696.7866 * diff14 - 1145.1508 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 + 107.9865 * contr12 + 78.5242 * contr13 + 3.2663 * contr14 + 115.4975 * contr23 - 125.6655 * contr24 + 109.004 * contr34 + 2963.8914 ;
}

double DistanceEstimator::LM9()
{
	return -734.6961 * diff12 - 29.9662 * diff13 + 677.8013 * diff14 + 120.9716 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 + 107.9865 * contr12 + 113.4276 * contr13 + 3.2663 * contr14 + 115.4975 * contr23 - 125.6655 * contr24 + 123.4555 * contr34 + 2655.7185 ;
}

double DistanceEstimator::LM10()
{
	return -5.6969 * diff12 + 149.8099 * diff13 + 202.1522 * diff14 + 79.0598 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 285.5559 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 145.8951 * contr23 - 403.4505 * contr24 - 113.8248 * contr34 + 2683.0255 ;
}

double DistanceEstimator::LM11()
{
	return -5.6969 * diff12 + 659.7795 * diff13 + 202.1522 * diff14 + 79.0598 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 710.5601 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 170.0379 * contr23 - 403.4505 * contr24 - 113.8248 * contr34 + 2705.8479 ;
}

double DistanceEstimator::LM12()
{
	return -5.6969 * diff12 + 918.1141 * diff13 + 202.1522 * diff14 + 79.0598 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 2357.8925 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 170.0379 * contr23 - 403.4505 * contr24 - 113.8248 * contr34 + 2883.5935 ;
}

double DistanceEstimator::LM13()
{
	return -5.6969 * diff12 + 726.1359 * diff13 + 202.1522 * diff14 + 79.0598 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 953.548 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 170.0379 * contr23 - 403.4505 * contr24 - 113.8248 * contr34 + 2748.4302 ;
}

double DistanceEstimator::LM14()
{
	return -5.6969 * diff12 + 726.1359 * diff13 + 202.1522 * diff14 + 79.0598 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 885.2825 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 170.0379 * contr23 - 403.4505 * contr24 - 113.8248 * contr34 + 2731.2215 ;
}

double DistanceEstimator::LM15()
{
	return -95.3941 * diff12 + 215.5391 * diff13 + 671.3208 * diff14 + 425.3819 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 509.694 * contr12 - 117.6827 * contr13 + 3.2663 * contr14 + 102.9747 * contr23 - 214.5137 * contr24 - 105.7863 * contr34 + 2466.973 ;
}

double DistanceEstimator::LM16()
{
	return -95.3941 * diff12 + 215.5391 * diff13 + 671.3208 * diff14 + 425.3819 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 560.7418 * contr12 - 117.6827 * contr13 + 3.2663 * contr14 + 102.9747 * contr23 - 214.5137 * contr24 - 105.7863 * contr34 + 2468.2972 ;
}

double DistanceEstimator::LM17()
{
	return -95.3941 * diff12 + 215.5391 * diff13 + 600.6964 * diff14 + 425.3819 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 530.9557 * contr12 - 117.6827 * contr13 + 3.2663 * contr14 + 102.9747 * contr23 - 214.5137 * contr24 - 217.8754 * contr34 + 2526.5359 ;
}

double DistanceEstimator::LM18()
{
	return -95.3941 * diff12 + 215.5391 * diff13 + 600.6964 * diff14 + 425.3819 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 768.6333 * contr12 - 117.6827 * contr13 + 3.2663 * contr14 + 102.9747 * contr23 - 214.5137 * contr24 - 174.8714 * contr34 + 2533.1627 ;
}

double DistanceEstimator::LM19()
{
	return -95.3941 * diff12 + 530.6942 * diff13 + 488.3951 * diff14 + 469.0959 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 168.2505 * contr12 - 117.6827 * contr13 + 3.2663 * contr14 + 102.9747 * contr23 - 214.5137 * contr24 + 126.6614 * contr34 + 2402.3341 ;
}

double DistanceEstimator::LM20()
{
	return -95.3941 * diff12 + 562.6836 * diff13 + 488.3951 * diff14 + 469.0959 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 168.2505 * contr12 - 117.6827 * contr13 + 3.2663 * contr14 + 102.9747 * contr23 - 214.5137 * contr24 + 126.6614 * contr34 + 2413.5234 ;
}

double DistanceEstimator::LM21()
{
	return -95.3941 * diff12 + 230.4636 * diff13 + 463.6813 * diff14 + 458.144 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 + 44.452 * contr12 - 146.9027 * contr13 + 2206.7959 * contr14 + 102.9747 * contr23 - 214.5137 * contr24 + 397.6657 * contr34 + 1833.1987 ;
}

double DistanceEstimator::LM22()
{
	return -946.1579 * diff12 + 319.7309 * diff13 + 211.3545 * diff14 + 271.2125 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 61.8612 * contr12 + 1133.4487 * contr13 + 3.2663 * contr14 + 102.9747 * contr23 - 776.2631 * contr24 + 160.4117 * contr34 + 2692.2263 ;
}

double DistanceEstimator::LM23()
{
	return -946.1579 * diff12 + 74.6743 * diff13 + 211.3545 * diff14 + 271.2125 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 61.8612 * contr12 + 873.3409 * contr13 + 3.2663 * contr14 + 102.9747 * contr23 - 627.0071 * contr24 + 160.4117 * contr34 + 2703.2502 ;
}

double DistanceEstimator::LM24()
{
	return -946.1579 * diff12 + 74.6743 * diff13 + 211.3545 * diff14 + 271.2125 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 61.8612 * contr12 + 873.3409 * contr13 + 3.2663 * contr14 + 102.9747 * contr23 - 633.4965 * contr24 + 160.4117 * contr34 + 2706.9986 ;
}

double DistanceEstimator::LM25()
{
	return -966.2933 * diff12 + 70.9467 * diff13 + 211.3545 * diff14 + 271.2125 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 584.5571 * contr12 + 437.0232 * contr13 + 3.2663 * contr14 + 102.9747 * contr23 - 793.2858 * contr24 + 160.4117 * contr34 + 2840.1524 ;
}

double DistanceEstimator::LM26()
{
	return -966.2933 * diff12 - 115.1895 * diff13 + 211.3545 * diff14 + 271.2125 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 584.5571 * contr12 + 437.0232 * contr13 + 3.2663 * contr14 + 102.9747 * contr23 - 793.2858 * contr24 + 160.4117 * contr34 + 2833.8065 ;
}

double DistanceEstimator::LM27()
{
	return -5.6969 * diff12 + 20.9145 * diff13 + 191.9271 * diff14 + 51.8341 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 75.3522 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 102.9747 * contr23 - 214.5137 * contr24 + 16.5697 * contr34 + 2546.687 ;
}

double DistanceEstimator::LM28()
{
	return -280.2593 * diff12 - 3.6148 * diff13 + 43.0173 * diff14 + 22.2033 * diff23 + 0.0679 * diff24 - 61.2888 * diff34 - 139.5434 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 176.9033 * contr23 - 224.5566 * contr24 + 85.2266 * contr34 + 2659.4374 ;
}

double DistanceEstimator::LM29()
{
	return -426.6361 * diff12 - 3.6148 * diff13 + 884.4431 * diff14 + 22.2033 * diff23 + 0.0679 * diff24 - 93.6387 * diff34 - 175.7867 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 176.9033 * contr23 - 224.5566 * contr24 + 85.2266 * contr34 + 2573.053 ;
}

double DistanceEstimator::LM30()
{
	return -117.7339 * diff12 - 3.6148 * diff13 - 263.2616 * diff14 + 22.2033 * diff23 + 0.0679 * diff24 - 146.9111 * diff34 + 25.688 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 176.9033 * contr23 - 224.5566 * contr24 + 85.2266 * contr34 + 2606.997 ;
}

double DistanceEstimator::LM31()
{
	return -163.6277 * diff12 - 3.6148 * diff13 - 263.2616 * diff14 + 22.2033 * diff23 + 0.0679 * diff24 - 146.9111 * diff34 + 25.688 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 176.9033 * contr23 - 224.5566 * contr24 + 85.2266 * contr34 + 2619.5508 ;
}

double DistanceEstimator::LM32()
{
	return -421.5641 * diff12 - 3.6148 * diff13 - 263.2616 * diff14 + 22.2033 * diff23 + 0.0679 * diff24 - 146.9111 * diff34 + 171.0326 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 176.9033 * contr23 - 224.5566 * contr24 + 85.2266 * contr34 + 2677.2494 ;
}

double DistanceEstimator::LM33()
{
	return -254.2936 * diff12 - 3.6148 * diff13 + 320.3752 * diff14 - 23.5341 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 366.5795 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 197.6455 * contr23 - 644.1263 * contr24 + 85.2266 * contr34 + 2805.6103 ;
}

double DistanceEstimator::LM34()
{
	return -254.2936 * diff12 - 3.6148 * diff13 + 320.3752 * diff14 - 52.4519 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 743.1988 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 197.6455 * contr23 - 644.1263 * contr24 + 85.2266 * contr34 + 2839.7638 ;
}

double DistanceEstimator::LM35()
{
	return -254.2936 * diff12 - 3.6148 * diff13 + 320.3752 * diff14 - 52.4519 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 1246.4013 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 197.6455 * contr23 - 644.1263 * contr24 + 85.2266 * contr34 + 2861.1992 ;
}

double DistanceEstimator::LM36()
{
	return -947.9794 * diff12 - 3.6148 * diff13 + 318.7217 * diff14 + 108.4936 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 279.8588 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 197.6455 * contr23 - 270.4821 * contr24 + 85.2266 * contr34 + 2780.9742 ;
}

double DistanceEstimator::LM37()
{
	return -1558.1322 * diff12 - 3.6148 * diff13 + 318.7217 * diff14 - 62.3049 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 279.8588 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 197.6455 * contr23 - 254.5417 * contr24 + 85.2266 * contr34 + 2950.4588 ;
}

double DistanceEstimator::LM38()
{
	return -883.1642 * diff12 - 3.6148 * diff13 + 318.7217 * diff14 + 322.6933 * diff23 + 0.0679 * diff24 - 26.0194 * diff34 - 279.8588 * contr12 - 3.6014 * contr13 + 3.2663 * contr14 + 197.6455 * contr23 - 176.8063 * contr24 + 85.2266 * contr34 + 2712.4439 ;
}

double DistanceEstimator::LM39()
{
	return -388.266 * diff12 - 457.9714 * diff13 + 195.9819 * diff14 - 21.8332 * diff23 + 0.0679 * diff24 - 229.1489 * diff34 + 324.2428 * contr12 + 933.529 * contr13 + 99.1027 * contr14 + 142.5288 * contr23 + 914.1029 * contr24 + 679.6772 * contr34 + 1917.3649 ;
}

double DistanceEstimator::LM40()
{
	return -388.266 * diff12 - 457.9714 * diff13 + 195.9819 * diff14 - 21.8332 * diff23 + 0.0679 * diff24 - 229.1489 * diff34 + 341.0362 * contr12 + 933.529 * contr13 + 99.1027 * contr14 + 142.5288 * contr23 + 914.1029 * contr24 + 679.6772 * contr34 + 1926.6433 ;
}

double DistanceEstimator::LM41()
{
	return -436.0871 * diff12 - 457.9714 * diff13 + 195.9819 * diff14 - 21.8332 * diff23 + 0.0679 * diff24 - 229.1489 * diff34 + 958.4835 * contr12 + 584.3295 * contr13 + 99.1027 * contr14 + 142.5288 * contr23 + 1038.3275 * contr24 + 679.6772 * contr34 + 1853.8266 ;
}

double DistanceEstimator::LM42()
{
	return -710.6853 * diff12 - 457.9714 * diff13 + 195.9819 * diff14 - 21.8332 * diff23 + 0.0679 * diff24 - 229.1489 * diff34 + 890.0183 * contr12 + 305.1597 * contr13 + 99.1027 * contr14 + 142.5288 * contr23 + 1038.3275 * contr24 + 679.6772 * contr34 + 1961.6944 ;
}

double DistanceEstimator::LM43()
{
	return -5.6969 * diff12 - 809.6453 * diff13 + 855.8144 * diff14 - 21.8332 * diff23 + 0.0679 * diff24 - 229.1489 * diff34 + 242.7045 * contr12 + 1798.5299 * contr13 + 99.1027 * contr14 + 142.5288 * contr23 + 562.8402 * contr24 + 516.6705 * contr34 + 1903.7912 ;
}

double DistanceEstimator::LM44()
{
	return -5.6969 * diff12 - 1517.5554 * diff13 + 195.9819 * diff14 - 21.8332 * diff23 + 0.0679 * diff24 - 229.1489 * diff34 + 242.7045 * contr12 + 1074.2839 * contr13 + 99.1027 * contr14 + 142.5288 * contr23 + 330.9286 * contr24 + 516.6705 * contr34 + 2284.2905 ;
}

double DistanceEstimator::LM45()
{
	return -5.6969 * diff12 - 842.2585 * diff13 + 195.9819 * diff14 - 21.8332 * diff23 + 0.0679 * diff24 - 229.1489 * diff34 + 242.7045 * contr12 + 3087.3874 * contr13 + 99.1027 * contr14 + 142.5288 * contr23 + 443.268 * contr24 + 516.6705 * contr34 + 1902.8245 ;
}

double DistanceEstimator::LM46()
{
	return -5.6969 * diff12 - 35.9192 * diff13 + 232.089 * diff14 - 21.8332 * diff23 + 514.9282 * diff24 - 850.7206 * diff34 + 227.8287 * contr12 + 222.532 * contr13 - 103.0215 * contr14 + 142.5288 * contr23 - 79.6935 * contr24 + 412.146 * contr34 + 2231.4552 ;
}

double DistanceEstimator::LM47()
{
	return -875.9503 * diff12 - 105.3295 * diff13 + 268.1325 * diff14 - 366.8417 * diff23 + 0.2206 * diff24 - 8.9567 * diff34 - 545.36 * contr12 - 478.388 * contr13 + 3.2663 * contr14 + 382.4762 * contr23 - 201.0265 * contr24 + 63.0273 * contr34 + 2853.3798 ;
}

double DistanceEstimator::LM48()
{
	return -875.9503 * diff12 - 105.3295 * diff13 + 268.1325 * diff14 - 366.8417 * diff23 + 0.2206 * diff24 - 8.9567 * diff34 - 478.3035 * contr12 - 478.388 * contr13 + 3.2663 * contr14 + 382.4762 * contr23 - 201.0265 * contr24 + 63.0273 * contr34 + 2842.5343 ;
}

double DistanceEstimator::LM49()
{
	return -875.9503 * diff12 - 98.9469 * diff13 + 268.1325 * diff14 - 366.8417 * diff23 + 0.2206 * diff24 - 8.9567 * diff34 - 216.6587 * contr12 - 1089.4625 * contr13 + 3.2663 * contr14 + 382.4762 * contr23 - 201.0265 * contr24 + 63.0273 * contr34 + 2811.9861 ;
}

double DistanceEstimator::LM50()
{
	return -875.9503 * diff12 - 136.9192 * diff13 + 268.1325 * diff14 - 366.8417 * diff23 + 0.2206 * diff24 - 8.9567 * diff34 - 139.4105 * contr12 - 394.0237 * contr13 + 3.2663 * contr14 + 382.4762 * contr23 - 201.0265 * contr24 + 63.0273 * contr34 + 2845.3764 ;
}

double DistanceEstimator::LM51()
{
	return -895.9172 * diff12 - 454.6366 * diff13 + 268.1325 * diff14 - 366.8417 * diff23 + 0.2206 * diff24 + 3596.4201 * diff34 + 326.3768 * contr12 - 2766.3293 * contr13 + 2447.9832 * contr14 + 4272.8627 * contr23 - 201.0265 * contr24 + 63.0273 * contr34 + 1612.2287 ;
}

double DistanceEstimator::LM52()
{
	return -895.9172 * diff12 - 454.6366 * diff13 + 268.1325 * diff14 - 366.8417 * diff23 + 0.2206 * diff24 + 1832.7582 * diff34 + 326.3768 * contr12 - 824.6628 * contr13 + 3.2663 * contr14 + 1566.5084 * contr23 - 201.0265 * contr24 + 63.0273 * contr34 + 2643.8881 ;
}

double DistanceEstimator::LM53()
{
	return -1344.0848 * diff12 + 74.5137 * diff13 + 268.1325 * diff14 - 333.2238 * diff23 + 0.2206 * diff24 - 701.4236 * diff34 + 586.4083 * contr12 - 756.4113 * contr13 + 3.2663 * contr14 + 286.6905 * contr23 - 201.0265 * contr24 + 63.0273 * contr34 + 2886.6999 ;
}

double DistanceEstimator::LM54()
{
	return -2065.9251 * diff12 - 803.5559 * diff13 + 268.1325 * diff14 + 413.2853 * diff23 + 0.2206 * diff24 - 701.4236 * diff34 + 306.2466 * contr12 - 1166.7246 * contr13 + 3.2663 * contr14 + 286.6905 * contr23 - 201.0265 * contr24 + 63.0273 * contr34 + 2914.2507 ;
}

double DistanceEstimator::LM55()
{
	return -655.5828 * diff12 - 352.4944 * diff13 + 268.1325 * diff14 - 2841.4505 * diff23 + 0.2206 * diff24 - 375.5252 * diff34 + 297.3507 * contr12 - 826.1016 * contr13 + 3.2663 * contr14 + 240.0008 * contr23 - 201.0265 * contr24 + 63.0273 * contr34 + 3529.1433 ;
}

double DistanceEstimator::LM56()
{
	return -246.4203 * diff12 - 105.4558 * diff13 + 647.6351 * diff14 - 284.2841 * diff23 + 0.2206 * diff24 + 135.9097 * diff34 + 44.6665 * contr12 - 119.5681 * contr13 + 3.2663 * contr14 + 109.4493 * contr23 - 252.9403 * contr24 + 55.7724 * contr34 + 2730.3497 ;
}

double DistanceEstimator::LM57()
{
	return -246.4203 * diff12 - 105.4558 * diff13 + 647.6351 * diff14 - 487.6984 * diff23 + 0.2206 * diff24 + 135.9097 * diff34 + 44.6665 * contr12 - 119.5681 * contr13 + 3.2663 * contr14 + 862.0588 * contr23 - 252.9403 * contr24 + 55.7724 * contr34 + 2731.5026 ;
}

double DistanceEstimator::LM58()
{
	return -223.8525 * diff12 - 105.4558 * diff13 + 647.6351 * diff14 - 78.2295 * diff23 + 0.2206 * diff24 + 135.9097 * diff34 + 44.6665 * contr12 - 119.5681 * contr13 + 3.2663 * contr14 + 0.7288 * contr23 - 252.9403 * contr24 + 55.7724 * contr34 + 2676.1531 ;
}

double DistanceEstimator::LM59()
{
	return -5.6969 * diff12 - 105.4558 * diff13 + 2153.5109 * diff14 - 135.6277 * diff23 + 0.2206 * diff24 + 99.3154 * diff34 + 44.6665 * contr12 - 119.5681 * contr13 + 3.2663 * contr14 + 14.3145 * contr23 - 252.9403 * contr24 + 55.7724 * contr34 + 2424.5566 ;
}

double DistanceEstimator::LM60()
{
	return -5.6969 * diff12 - 105.4558 * diff13 + 1006.5375 * diff14 + 736.4976 * diff23 + 0.2206 * diff24 - 357.1228 * diff34 + 44.6665 * contr12 - 119.5681 * contr13 + 3.2663 * contr14 + 777.1952 * contr23 - 252.9403 * contr24 + 55.7724 * contr34 + 2470.3043 ;
}

double DistanceEstimator::LM61()
{
	return -5.6969 * diff12 - 105.4558 * diff13 + 1006.5375 * diff14 + 923.4059 * diff23 + 0.2206 * diff24 - 357.1228 * diff34 + 44.6665 * contr12 - 119.5681 * contr13 + 3.2663 * contr14 + 777.1952 * contr23 - 252.9403 * contr24 + 55.7724 * contr34 + 2436.1091 ;
}

double DistanceEstimator::LM62()
{
	return -5.6969 * diff12 - 105.4558 * diff13 + 1006.5375 * diff14 - 322.6491 * diff23 + 0.2206 * diff24 - 298.041 * diff34 + 44.6665 * contr12 - 119.5681 * contr13 + 3.2663 * contr14 + 445.6598 * contr23 - 252.9403 * contr24 + 55.7724 * contr34 + 2692.1929 ;
}

double DistanceEstimator::LM63()
{
	return -5.6969 * diff12 - 105.4558 * diff13 + 1006.5375 * diff14 - 322.6491 * diff23 + 0.2206 * diff24 - 282.9049 * diff34 + 44.6665 * contr12 - 119.5681 * contr13 + 3.2663 * contr14 + 445.6598 * contr23 - 252.9403 * contr24 + 55.7724 * contr34 + 2685.7369 ;
}

double DistanceEstimator::LM64()
{
	return -5.6969 * diff12 + 106.9025 * diff13 + 842.1881 * diff14 + 54.9048 * diff23 + 0.2206 * diff24 - 19.3386 * diff34 - 158.0428 * contr12 - 119.5681 * contr13 - 808.5498 * contr14 - 31.5271 * contr23 - 487.1812 * contr24 + 55.7724 * contr34 + 2898.136 ;
}

double DistanceEstimator::LM65()
{
	return -5.6969 * diff12 + 684.5468 * diff13 + 767.806 * diff14 + 54.9048 * diff23 + 0.2206 * diff24 - 19.3386 * diff34 - 158.0428 * contr12 - 119.5681 * contr13 - 1275.3145 * contr14 - 31.5271 * contr23 - 487.1812 * contr24 + 55.7724 * contr34 + 3019.9783 ;
}

double DistanceEstimator::LM66()
{
	return -5.6969 * diff12 + 225.8982 * diff13 + 767.806 * diff14 + 54.9048 * diff23 + 0.2206 * diff24 - 19.3386 * diff34 - 158.0428 * contr12 - 119.5681 * contr13 - 1119.0128 * contr14 - 31.5271 * contr23 - 487.1812 * contr24 + 55.7724 * contr34 + 3010.8113 ;
}

double DistanceEstimator::LM67()
{
	return -164.229 * diff12 - 107.4983 * diff13 + 722.6055 * diff14 - 611.9015 * diff23 + 0.2206 * diff24 - 19.3386 * diff34 - 381.3735 * contr12 - 119.5681 * contr13 - 176.3043 * contr14 - 31.5271 * contr23 - 601.9631 * contr24 + 55.7724 * contr34 + 2947.2195 ;
}

double DistanceEstimator::LM68()
{
	return -164.229 * diff12 - 107.4983 * diff13 + 722.6055 * diff14 - 579.2442 * diff23 + 0.2206 * diff24 - 19.3386 * diff34 - 381.3735 * contr12 - 119.5681 * contr13 - 176.3043 * contr14 - 31.5271 * contr23 - 601.9631 * contr24 + 55.7724 * contr34 + 2931.6124 ;
}

double DistanceEstimator::LM69()
{
	return -164.229 * diff12 - 107.4983 * diff13 + 722.6055 * diff14 - 579.2442 * diff23 + 0.2206 * diff24 - 19.3386 * diff34 + 12.5799 * contr12 - 119.5681 * contr13 - 176.3043 * contr14 - 31.5271 * contr23 - 601.9631 * contr24 + 55.7724 * contr34 + 2906.2196 ;
}

double DistanceEstimator::LM70()
{
	return -164.229 * diff12 - 107.4983 * diff13 + 722.6055 * diff14 - 579.2442 * diff23 + 0.2206 * diff24 - 19.3386 * diff34 - 62.4588 * contr12 - 119.5681 * contr13 - 176.3043 * contr14 - 31.5271 * contr23 - 601.9631 * contr24 + 55.7724 * contr34 + 2912.5351 ;
}

double DistanceEstimator::LM71()
{
	return -278.174 * diff12 - 107.4983 * diff13 + 722.6055 * diff14 - 464.928 * diff23 + 0.2206 * diff24 - 19.3386 * diff34 - 500.3781 * contr12 - 119.5681 * contr13 - 305.3706 * contr14 - 31.5271 * contr23 - 601.9631 * contr24 + 55.7724 * contr34 + 2941.2634 ;
}

double DistanceEstimator::LM72()
{
	return -5.6969 * diff12 - 107.4983 * diff13 + 1722.0948 * diff14 - 68.6174 * diff23 + 0.2206 * diff24 - 19.3386 * diff34 - 171.8649 * contr12 - 119.5681 * contr13 + 322.3182 * contr14 - 31.5271 * contr23 - 538.3903 * contr24 + 55.7724 * contr34 + 2447.1988 ;
}

double DistanceEstimator::LM73()
{
	return -998.8312 * diff12 - 37.0507 * diff13 + 98.837 * diff14 + 558.7406 * diff23 + 0.2206 * diff24 - 19.3386 * diff34 - 717.3544 * contr12 - 36.3595 * contr13 + 16.0951 * contr14 + 627.2005 * contr23 - 47.7394 * contr24 + 21.496 * contr34 + 2669.4124 ;
}

double DistanceEstimator::LM74()
{
	return -215.976 * diff12 - 37.0507 * diff13 + 98.837 * diff14 - 2146.4099 * diff23 + 0.2206 * diff24 - 616.9114 * diff34 + 326.0574 * contr12 - 36.3595 * contr13 + 718.891 * contr14 + 33.2068 * contr23 - 47.7394 * contr24 - 121.5405 * contr34 + 3091.263 ;
}

double DistanceEstimator::LM75()
{
	return -215.976 * diff12 - 37.0507 * diff13 + 98.837 * diff14 + 118.0711 * diff23 + 0.2206 * diff24 - 616.9114 * diff34 + 958.923 * contr12 - 36.3595 * contr13 + 1305.3793 * contr14 + 33.2068 * contr23 - 47.7394 * contr24 - 145.704 * contr34 + 2397.2279 ;
}

double DistanceEstimator::LM76()
{
	return -215.976 * diff12 - 37.0507 * diff13 + 98.837 * diff14 + 1291.7592 * diff23 + 0.2206 * diff24 - 616.9114 * diff34 + 958.923 * contr12 - 36.3595 * contr13 + 2198.344 * contr14 + 33.2068 * contr23 - 47.7394 * contr24 - 145.704 * contr34 + 1957.9193 ;
}

double DistanceEstimator::LM77()
{
	return -215.976 * diff12 - 37.0507 * diff13 + 98.837 * diff14 - 1683.5184 * diff23 + 0.2206 * diff24 - 2505.5522 * diff34 + 286.0484 * contr12 - 36.3595 * contr13 + 259.7884 * contr14 + 33.2068 * contr23 - 47.7394 * contr24 - 3.408 * contr34 + 3158.9101 ;
}

double DistanceEstimator::LM78()
{
	return -215.976 * diff12 - 37.0507 * diff13 + 98.837 * diff14 - 1683.5184 * diff23 + 0.2206 * diff24 - 2256.8244 * diff34 + 286.0484 * contr12 - 36.3595 * contr13 + 273.4692 * contr14 + 33.2068 * contr23 - 47.7394 * contr24 - 3.408 * contr34 + 3130.0493 ;
}

double DistanceEstimator::LM79()
{
	return -215.976 * diff12 - 37.0507 * diff13 + 98.837 * diff14 - 1683.5184 * diff23 + 0.2206 * diff24 - 2256.8244 * diff34 + 286.0484 * contr12 - 36.3595 * contr13 + 288.3048 * contr14 + 33.2068 * contr23 - 47.7394 * contr24 - 3.408 * contr34 + 3130.0769 ;
}

double DistanceEstimator::LM80()
{
	return -215.976 * diff12 - 37.0507 * diff13 + 98.837 * diff14 - 1683.5184 * diff23 + 0.2206 * diff24 - 2409.2826 * diff34 + 286.0484 * contr12 - 36.3595 * contr13 + 290.3603 * contr14 + 33.2068 * contr23 - 47.7394 * contr24 - 3.408 * contr34 + 3157.641 ;
}

double DistanceEstimator::LM81()
{
	return -1284.702 * diff12 - 37.0507 * diff13 + 98.837 * diff14 - 708.6008 * diff23 + 0.2206 * diff24 - 19.3386 * diff34 + 383.8071 * contr12 - 36.3595 * contr13 + 169.0355 * contr14 + 33.2068 * contr23 - 47.7394 * contr24 - 3.408 * contr34 + 2982.2066 ;
}

double DistanceEstimator::LM82()
{
	return -11.924 * diff12 - 214.5878 * diff13 + 2178.2177 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 67.3057 * diff34 - 556.7188 * contr12 + 134.2101 * contr13 - 109.6619 * contr14 + 96.1584 * contr23 - 86.6425 * contr24 + 184.5386 * contr34 + 2321.8807 ;
}

double DistanceEstimator::LM83()
{
	return -11.924 * diff12 - 214.5878 * diff13 + 2178.2177 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 67.3057 * diff34 - 889.9371 * contr12 + 375.3743 * contr13 - 109.6619 * contr14 + 119.1239 * contr23 - 86.6425 * contr24 + 205.442 * contr34 + 2317.4879 ;
}

double DistanceEstimator::LM84()
{
	return -11.924 * diff12 - 214.5878 * diff13 + 2178.2177 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 67.3057 * diff34 - 845.7651 * contr12 + 342.6943 * contr13 - 109.6619 * contr14 + 119.1239 * contr23 - 86.6425 * contr24 + 205.442 * contr34 + 2319.497 ;
}

double DistanceEstimator::LM85()
{
	return -11.924 * diff12 - 86.6288 * diff13 + 1251.902 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 67.3057 * diff34 - 227.5604 * contr12 + 139.1538 * contr13 + 56.1872 * contr14 - 42.1111 * contr23 - 86.6425 * contr24 + 144.6515 * contr34 + 2531.5363 ;
}

double DistanceEstimator::LM86()
{
	return -11.924 * diff12 - 86.6288 * diff13 + 1251.902 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 67.3057 * diff34 - 227.5604 * contr12 + 229.8445 * contr13 - 3.2489 * contr14 - 42.1111 * contr23 - 86.6425 * contr24 + 150.0697 * contr34 + 2539.6733 ;
}

double DistanceEstimator::LM87()
{
	return -11.924 * diff12 - 86.6288 * diff13 + 1251.902 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 67.3057 * diff34 - 227.5604 * contr12 + 184.4962 * contr13 - 3.2489 * contr14 - 42.1111 * contr23 - 86.6425 * contr24 + 108.1539 * contr34 + 2544.7684 ;
}

double DistanceEstimator::LM88()
{
	return -11.924 * diff12 - 86.6288 * diff13 + 1251.902 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 67.3057 * diff34 - 227.5604 * contr12 + 161.0119 * contr13 - 3.2489 * contr14 - 42.1111 * contr23 - 86.6425 * contr24 + 108.1539 * contr34 + 2542.6383 ;
}

double DistanceEstimator::LM89()
{
	return -134.2953 * diff12 - 86.6288 * diff13 + 1251.902 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 67.3057 * diff34 - 227.5604 * contr12 + 161.0119 * contr13 - 3.2489 * contr14 - 42.1111 * contr23 - 86.6425 * contr24 + 108.1539 * contr34 + 2556.2016 ;
}

double DistanceEstimator::LM90()
{
	return -134.2953 * diff12 - 86.6288 * diff13 + 1251.902 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 67.3057 * diff34 - 227.5604 * contr12 + 161.0119 * contr13 - 3.2489 * contr14 - 42.1111 * contr23 - 86.6425 * contr24 + 108.1539 * contr34 + 2554.7858 ;
}

double DistanceEstimator::LM91()
{
	return -245.0343 * diff12 - 5.8126 * diff13 - 3761.1629 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 172.4978 * diff34 - 3201.4861 * contr12 + 46.8414 * contr13 - 59.7579 * contr14 - 103.1799 * contr23 - 86.6425 * contr24 + 17.311 * contr34 + 4029.5039 ;
}

double DistanceEstimator::LM92()
{
	return -11.924 * diff12 - 5.8126 * diff13 - 5375.9475 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 1922.6887 * diff34 - 1983.1196 * contr12 + 46.8414 * contr13 - 59.7579 * contr14 - 103.1799 * contr23 - 86.6425 * contr24 + 17.311 * contr34 + 4578.6364 ;
}

double DistanceEstimator::LM93()
{
	return -11.924 * diff12 - 5.8126 * diff13 - 5375.9475 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 2167.5058 * diff34 - 1983.1196 * contr12 + 46.8414 * contr13 - 59.7579 * contr14 - 103.1799 * contr23 - 86.6425 * contr24 + 17.311 * contr34 + 4587.4636 ;
}

double DistanceEstimator::LM94()
{
	return -11.924 * diff12 - 5.8126 * diff13 - 3796.2732 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 725.9621 * diff34 - 1983.1196 * contr12 + 46.8414 * contr13 - 59.7579 * contr14 - 103.1799 * contr23 - 86.6425 * contr24 + 17.311 * contr34 + 4061.7248 ;
}

double DistanceEstimator::LM95()
{
	return -11.924 * diff12 - 5.8126 * diff13 - 3393.1911 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 725.9621 * diff34 - 1983.1196 * contr12 + 46.8414 * contr13 - 59.7579 * contr14 - 103.1799 * contr23 - 86.6425 * contr24 + 17.311 * contr34 + 3949.7312 ;
}

double DistanceEstimator::LM96()
{
	return 149.4991 * diff12 - 5.8126 * diff13 + 438.9701 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 9.1976 * diff34 + 1650.7212 * contr12 - 62.98 * contr13 + 550.9855 * contr14 + 44.0064 * contr23 - 86.6425 * contr24 + 17.311 * contr34 + 2465.0167 ;
}

double DistanceEstimator::LM97()
{
	return 149.4991 * diff12 - 5.8126 * diff13 + 438.9701 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 9.1976 * diff34 + 1774.5355 * contr12 - 62.98 * contr13 + 550.9855 * contr14 + 44.0064 * contr23 - 86.6425 * contr24 + 17.311 * contr34 + 2458.2762 ;
}

double DistanceEstimator::LM98()
{
	return 149.4991 * diff12 - 5.8126 * diff13 + 438.9701 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 9.1976 * diff34 + 673.9456 * contr12 - 62.98 * contr13 + 364.1714 * contr14 + 44.0064 * contr23 - 86.6425 * contr24 + 17.311 * contr34 + 2589.1567 ;
}

double DistanceEstimator::LM99()
{
	return 81.0142 * diff12 - 5.8126 * diff13 + 438.9701 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 9.1976 * diff34 + 38.5791 * contr12 - 62.98 * contr13 + 64.8094 * contr14 + 44.0064 * contr23 - 86.6425 * contr24 + 17.311 * contr34 + 2720.9287 ;
}

double DistanceEstimator::LM100()
{
	return 69.9495 * diff12 - 5.8126 * diff13 + 438.9701 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 9.1976 * diff34 - 20.6097 * contr12 - 62.98 * contr13 + 29.7999 * contr14 + 44.0064 * contr23 - 86.6425 * contr24 + 17.311 * contr34 + 2753.9474 ;
}

double DistanceEstimator::LM101()
{
	return -11.924 * diff12 - 5.8126 * diff13 + 1802.8283 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 891.0722 * diff34 - 79.4363 * contr12 - 372.2168 * contr13 + 35.1819 * contr14 + 36.4766 * contr23 - 470.7682 * contr24 + 17.311 * contr34 + 2518.5135 ;
}

double DistanceEstimator::LM102()
{
	return -11.924 * diff12 - 5.8126 * diff13 + 1802.8283 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 1881.11 * diff34 - 79.4363 * contr12 - 392.6596 * contr13 + 35.1819 * contr14 + 36.4766 * contr23 - 470.7682 * contr24 + 17.311 * contr34 + 2471.3608 ;
}

double DistanceEstimator::LM103()
{
	return -11.924 * diff12 - 5.8126 * diff13 + 1802.8283 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 1898.6794 * diff34 - 79.4363 * contr12 - 392.6596 * contr13 + 35.1819 * contr14 + 36.4766 * contr23 - 257.6262 * contr24 + 17.311 * contr34 + 2410.015 ;
}

double DistanceEstimator::LM104()
{
	return -11.924 * diff12 - 5.8126 * diff13 + 1802.8283 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 1526.612 * diff34 - 79.4363 * contr12 - 392.6596 * contr13 + 35.1819 * contr14 + 36.4766 * contr23 - 470.7682 * contr24 + 17.311 * contr34 + 2486.1334 ;
}

double DistanceEstimator::LM105()
{
	return -11.924 * diff12 - 5.8126 * diff13 - 2249.0333 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 45.176 * diff34 - 79.4363 * contr12 - 848.6159 * contr13 + 70.0824 * contr14 + 36.4766 * contr23 - 412.4845 * contr24 + 17.311 * contr34 + 3749.875 ;
}

double DistanceEstimator::LM106()
{
	return -11.924 * diff12 - 5.8126 * diff13 - 3012.0367 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 45.176 * diff34 - 79.4363 * contr12 - 968.4403 * contr13 + 70.0824 * contr14 + 36.4766 * contr23 - 412.4845 * contr24 + 17.311 * contr34 + 3974.0557 ;
}

double DistanceEstimator::LM107()
{
	return -11.924 * diff12 - 5.8126 * diff13 + 1565.9838 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 45.176 * diff34 - 79.4363 * contr12 - 249.4936 * contr13 + 150.897 * contr14 + 36.4766 * contr23 - 412.4845 * contr24 + 17.311 * contr34 + 2563.3893 ;
}

double DistanceEstimator::LM108()
{
	return -11.924 * diff12 - 5.8126 * diff13 + 1565.9838 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 45.176 * diff34 - 79.4363 * contr12 - 249.4936 * contr13 + 136.2034 * contr14 + 36.4766 * contr23 - 412.4845 * contr24 + 17.311 * contr34 + 2570.644 ;
}

double DistanceEstimator::LM109()
{
	return -11.924 * diff12 - 5.8126 * diff13 + 1565.9838 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 793.6253 * diff34 - 79.4363 * contr12 - 249.4936 * contr13 + 136.2034 * contr14 + 36.4766 * contr23 - 412.4845 * contr24 + 17.311 * contr34 + 2614.893 ;
}

double DistanceEstimator::LM110()
{
	return -11.924 * diff12 - 5.8126 * diff13 + 1565.9838 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 122.1021 * diff34 - 79.4363 * contr12 - 249.4936 * contr13 + 83.5418 * contr14 + 36.4766 * contr23 - 412.4845 * contr24 + 17.311 * contr34 + 2608.7584 ;
}

double DistanceEstimator::LM111()
{
	return -11.924 * diff12 - 676.1387 * diff13 + 1327.2617 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 123.5612 * diff34 - 99.5112 * contr12 - 521.8505 * contr13 + 37.1459 * contr14 + 36.4766 * contr23 - 998.4829 * contr24 + 17.311 * contr34 + 3039.6319 ;
}

double DistanceEstimator::LM112()
{
	return -11.924 * diff12 - 968.0188 * diff13 + 1327.2617 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 123.5612 * diff34 - 99.5112 * contr12 - 526.3161 * contr13 + 37.1459 * contr14 + 36.4766 * contr23 - 824.4751 * contr24 + 17.311 * contr34 + 3066.1459 ;
}

double DistanceEstimator::LM113()
{
	return -11.924 * diff12 - 1095.1322 * diff13 + 1327.2617 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 123.5612 * diff34 - 99.5112 * contr12 - 526.3161 * contr13 + 37.1459 * contr14 + 36.4766 * contr23 - 741.6987 * contr24 + 17.311 * contr34 + 3078.0723 ;
}

double DistanceEstimator::LM114()
{
	return -11.924 * diff12 - 545.0522 * diff13 + 1327.2617 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 123.5612 * diff34 - 99.5112 * contr12 - 370.0215 * contr13 + 37.1459 * contr14 + 36.4766 * contr23 - 660.5269 * contr24 + 17.311 * contr34 + 2906.2294 ;
}

double DistanceEstimator::LM115()
{
	return -11.924 * diff12 - 322.2989 * diff13 + 1327.2617 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 + 123.5612 * diff34 - 99.5112 * contr12 - 492.9112 * contr13 + 70.5406 * contr14 + 36.4766 * contr23 - 897.7747 * contr24 + 17.311 * contr34 + 2906.2656 ;
}

double DistanceEstimator::LM116()
{
	return -11.924 * diff12 - 5.8126 * diff13 + 5912.1196 * diff14 - 72.1885 * diff23 + 7.0596 * diff24 - 9.1976 * diff34 - 37.1654 * contr12 - 89.5734 * contr13 - 4.3095 * contr14 + 36.4766 * contr23 - 141.7288 * contr24 + 17.311 * contr34 + 1240.8829 ;
}

double DistanceEstimator::LM117()
{
	return 2047.8584 * diff12 + 3096.4577 * diff13 + 32.0442 * diff14 - 244.8933 * diff23 + 7.0596 * diff24 - 152.0515 * diff34 + 105.4772 * contr12 - 12.7177 * contr13 + 3.2663 * contr14 - 2824.8912 * contr23 - 48.5177 * contr24 + 80.3668 * contr34 + 2556.504 ;
}

double DistanceEstimator::LM118()
{
	return 2047.8584 * diff12 + 3096.4577 * diff13 + 32.0442 * diff14 - 244.8933 * diff23 + 7.0596 * diff24 - 152.0515 * diff34 + 105.4772 * contr12 - 12.7177 * contr13 + 3.2663 * contr14 - 9301.8991 * contr23 - 48.5177 * contr24 + 80.3668 * contr34 + 2808.1873 ;
}

double DistanceEstimator::LM119()
{
	return 664.7243 * diff12 + 2043.2254 * diff13 + 32.0442 * diff14 - 244.8933 * diff23 + 7.0596 * diff24 - 152.0515 * diff34 + 105.4772 * contr12 - 12.7177 * contr13 + 3.2663 * contr14 - 320.668 * contr23 - 48.5177 * contr24 + 80.3668 * contr34 + 2679.7924 ;
}

double DistanceEstimator::LM120()
{
	return -217.1143 * diff12 - 5.8126 * diff13 + 220.9606 * diff14 - 548.683 * diff23 + 7.0596 * diff24 - 60.679 * diff34 + 221.8489 * contr12 - 89.8617 * contr13 + 57.4433 * contr14 - 45.8891 * contr23 - 169.1174 * contr24 + 67.0384 * contr34 + 3009.3687 ;
}

double DistanceEstimator::LM121()
{
	return -8.1694 * diff12 + 26.3877 * diff13 + 182.473 * diff14 - 635.558 * diff23 + 7.0596 * diff24 - 60.679 * diff34 + 195.2421 * contr12 - 89.8617 * contr13 + 57.4433 * contr14 - 276.8092 * contr23 - 169.1174 * contr24 + 67.0384 * contr34 + 3007.3897 ;
}

double DistanceEstimator::LM122()
{
	return 24.2986 * diff12 + 26.3877 * diff13 + 159.4423 * diff14 - 635.558 * diff23 + 7.0596 * diff24 - 60.679 * diff34 + 190.0034 * contr12 - 89.8617 * contr13 + 57.4433 * contr14 - 276.8092 * contr23 - 169.1174 * contr24 + 67.0384 * contr34 + 3022.4189 ;
}

double DistanceEstimator::LM123()
{
	return -98.0654 * diff12 + 142.6744 * diff13 + 2499.8955 * diff14 - 635.558 * diff23 + 7.0596 * diff24 - 60.679 * diff34 + 219.6896 * contr12 - 89.8617 * contr13 + 57.4433 * contr14 - 458.3137 * contr23 - 169.1174 * contr24 + 67.0384 * contr34 + 2292.1707 ;
}

double DistanceEstimator::LM124()
{
	return -98.0654 * diff12 + 188.1778 * diff13 + 2091.6057 * diff14 - 635.558 * diff23 + 7.0596 * diff24 - 60.679 * diff34 + 219.6896 * contr12 - 89.8617 * contr13 + 57.4433 * contr14 - 458.3137 * contr23 - 169.1174 * contr24 + 67.0384 * contr34 + 2411.4752 ;
}

double DistanceEstimator::LM125()
{
	return -1453.2456 * diff12 + 26.0781 * diff13 + 126.7977 * diff14 - 633.4943 * diff23 - 77.5475 * diff24 - 60.679 * diff34 + 354.7923 * contr12 - 89.8617 * contr13 + 57.4433 * contr14 - 250.687 * contr23 - 169.1174 * contr24 + 67.0384 * contr34 + 3087.37 ;
}

double DistanceEstimator::LM126()
{
	return -663.7464 * diff12 + 26.0781 * diff13 + 126.7977 * diff14 - 633.4943 * diff23 - 181.8482 * diff24 - 60.679 * diff34 + 335.5437 * contr12 - 89.8617 * contr13 + 57.4433 * contr14 + 273.3585 * contr23 - 169.1174 * contr24 + 67.0384 * contr34 + 3055.3509 ;
}

double DistanceEstimator::LM127()
{
	return -663.7464 * diff12 + 26.0781 * diff13 + 126.7977 * diff14 - 633.4943 * diff23 - 406.1356 * diff24 - 60.679 * diff34 + 335.5437 * contr12 - 89.8617 * contr13 + 57.4433 * contr14 + 1238.7805 * contr23 - 169.1174 * contr24 + 67.0384 * contr34 + 3075.3588 ;
}

double DistanceEstimator::LM128()
{
	return -535.3643 * diff12 + 26.0781 * diff13 + 126.7977 * diff14 - 633.4943 * diff23 + 7.0596 * diff24 + 114.6642 * diff34 + 269.1945 * contr12 - 89.8617 * contr13 + 57.4433 * contr14 + 953.7726 * contr23 - 169.1174 * contr24 + 67.0384 * contr34 + 2965.5531 ;
}

double DistanceEstimator::LM129()
{
	return -535.3643 * diff12 + 26.0781 * diff13 + 126.7977 * diff14 - 633.4943 * diff23 + 7.0596 * diff24 + 63.7581 * diff34 + 269.1945 * contr12 - 89.8617 * contr13 + 57.4433 * contr14 + 335.6899 * contr23 - 169.1174 * contr24 + 67.0384 * contr34 + 2987.7466 ;
}

double DistanceEstimator::LM130()
{
	return -535.3643 * diff12 + 26.0781 * diff13 + 126.7977 * diff14 - 633.4943 * diff23 + 7.0596 * diff24 - 60.679 * diff34 + 269.1945 * contr12 - 89.8617 * contr13 + 57.4433 * contr14 - 84.8147 * contr23 - 169.1174 * contr24 + 67.0384 * contr34 + 3002.2885 ;
}

double DistanceEstimator::LM131()
{
	return -604.932 * diff12 - 5.8126 * diff13 + 98.6372 * diff14 - 545.0335 * diff23 + 7.0596 * diff24 + 39.7172 * diff34 + 370.5444 * contr12 - 89.8617 * contr13 + 37.7957 * contr14 + 151.1519 * contr23 - 169.1174 * contr24 + 205.5858 * contr34 + 2928.2324 ;
}

double DistanceEstimator::LM132()
{
	return -604.932 * diff12 - 5.8126 * diff13 + 98.6372 * diff14 - 545.0335 * diff23 + 7.0596 * diff24 + 39.7172 * diff34 + 370.5444 * contr12 - 89.8617 * contr13 + 37.7957 * contr14 + 151.1519 * contr23 - 169.1174 * contr24 + 217.8779 * contr34 + 2928.2413 ;
}

double DistanceEstimator::LM133()
{
	return -1419.2709 * diff12 - 5.8126 * diff13 + 98.6372 * diff14 - 545.0335 * diff23 + 7.0596 * diff24 + 14.6181 * diff34 + 370.5444 * contr12 - 89.8617 * contr13 + 54.2615 * contr14 + 553.1727 * contr23 - 169.1174 * contr24 + 166.9537 * contr34 + 2974.5046 ;
}

double DistanceEstimator::LM134()
{
	return -1279.2786 * diff12 - 5.8126 * diff13 + 98.6372 * diff14 - 545.0335 * diff23 + 7.0596 * diff24 + 14.6181 * diff34 + 370.5444 * contr12 - 89.8617 * contr13 + 54.2615 * contr14 + 429.474 * contr23 - 169.1174 * contr24 + 166.9537 * contr34 + 2979.1664 ;
}

double DistanceEstimator::LM135()
{
	return -1243.342 * diff12 - 5.8126 * diff13 + 98.6372 * diff14 - 545.0335 * diff23 + 7.0596 * diff24 + 14.6181 * diff34 + 370.5444 * contr12 - 89.8617 * contr13 + 367.3983 * contr14 + 151.1519 * contr23 - 169.1174 * contr24 + 166.9537 * contr34 + 2929.3252 ;
}

double DistanceEstimator::LM136()
{
	return -581.4139 * diff12 - 5.8126 * diff13 + 98.6372 * diff14 - 545.0335 * diff23 + 7.0596 * diff24 - 60.679 * diff34 + 370.5444 * contr12 - 89.8617 * contr13 + 52.2168 * contr14 + 151.1519 * contr23 - 169.1174 * contr24 + 166.9537 * contr34 + 2944.9438 ;
}

double DistanceEstimator::LM137()
{
	return 731.8586 * diff12 - 5.8126 * diff13 + 32.0442 * diff14 - 294.925 * diff23 + 591.7157 * diff24 - 74.4294 * diff34 - 94.4757 * contr12 - 163.6278 * contr13 + 3.2663 * contr14 - 29.6238 * contr23 - 310.8612 * contr24 + 180.5423 * contr34 + 2806.2902 ;
}

double DistanceEstimator::LM138()
{
	return 229.5148 * diff12 - 5.8126 * diff13 + 32.0442 * diff14 - 294.925 * diff23 - 564.3767 * diff24 - 74.4294 * diff34 - 179.3206 * contr12 - 163.6278 * contr13 + 3.2663 * contr14 - 29.6238 * contr23 - 310.8612 * contr24 + 191.0469 * contr34 + 3072.6387 ;
}

double DistanceEstimator::LM139()
{
	return 88.8453 * diff12 - 5.8126 * diff13 + 32.0442 * diff14 - 294.925 * diff23 - 398.2338 * diff24 - 74.4294 * diff34 - 1385.7444 * contr12 - 163.6278 * contr13 + 3.2663 * contr14 - 29.6238 * contr23 - 310.8612 * contr24 + 304.2633 * contr34 + 3045.5193 ;
}

double DistanceEstimator::LM140()
{
	return 868.0848 * diff12 - 5.8126 * diff13 + 32.0442 * diff14 - 294.925 * diff23 + 7.0596 * diff24 - 74.4294 * diff34 + 405.2947 * contr12 - 163.6278 * contr13 + 3.2663 * contr14 + 158.3441 * contr23 - 310.8612 * contr24 - 12.0425 * contr34 + 2947.7451 ;
}

double DistanceEstimator::LM141()
{
	return 1188.2718 * diff12 - 5.8126 * diff13 + 32.0442 * diff14 - 294.925 * diff23 + 7.0596 * diff24 - 74.4294 * diff34 + 510.7272 * contr12 - 163.6278 * contr13 + 3.2663 * contr14 + 709.8958 * contr23 - 310.8612 * contr24 - 12.0425 * contr34 + 2907.6789 ;
}

double DistanceEstimator::LM142()
{
	return 905.6172 * diff12 - 5.8126 * diff13 + 32.0442 * diff14 - 294.925 * diff23 + 7.0596 * diff24 - 74.4294 * diff34 + 510.7272 * contr12 - 163.6278 * contr13 + 3.2663 * contr14 + 616.197 * contr23 - 310.8612 * contr24 - 12.0425 * contr34 + 2945.9237 ;
}

double DistanceEstimator::LM143()
{
	return 17.3436 * diff12 - 5.8126 * diff13 + 32.0442 * diff14 - 294.925 * diff23 + 7.0596 * diff24 - 74.4294 * diff34 + 321.8614 * contr12 + 592.4084 * contr13 + 3.2663 * contr14 - 29.6238 * contr23 - 310.8612 * contr24 - 12.0425 * contr34 + 3035.8778 ;
}

double DistanceEstimator::LM144()
{
	return 2081.4973 * diff12 - 933.6179 * diff13 + 69.0059 * diff14 - 41.7422 * diff23 + 7.0596 * diff24 + 7.7381 * diff34 + 8.655 * contr12 - 84.8337 * contr13 + 143.709 * contr14 + 36.1924 * contr23 - 41.741 * contr24 + 22.1706 * contr34 + 2854.1768 ;
}

double DistanceEstimator::LM145()
{
	return -129.9397 * diff12 - 5.8126 * diff13 + 860.9977 * diff14 - 1138.02 * diff23 + 7.0596 * diff24 - 927.3815 * diff34 - 40.1944 * contr12 + 1102.1721 * contr13 - 3.5521 * contr14 + 195.077 * contr23 - 687.8155 * contr24 + 160.6044 * contr34 + 3041.3002 ;
}

double DistanceEstimator::LM146()
{
	return 109.6286 * diff12 + 73.3484 * diff13 + 53.491 * diff14 - 544.1397 * diff23 - 213.5889 * diff24 - 289.3493 * diff34 - 40.1944 * contr12 - 483.9983 * contr13 + 373.3725 * contr14 + 195.077 * contr23 - 1361.4999 * contr24 + 160.6044 * contr34 + 3258.9158 ;
}

double DistanceEstimator::LM147()
{
	return 342.1208 * diff12 + 73.3484 * diff13 + 53.491 * diff14 - 544.1397 * diff23 - 213.5889 * diff24 - 289.3493 * diff34 - 40.1944 * contr12 - 483.9983 * contr13 + 693.0664 * contr14 + 195.077 * contr23 - 1361.4999 * contr24 + 160.6044 * contr34 + 3158.3047 ;
}

double DistanceEstimator::LM148()
{
	return -565.4192 * diff12 + 93.5597 * diff13 + 53.491 * diff14 - 544.1397 * diff23 - 269.9247 * diff24 - 289.3493 * diff34 - 40.1944 * contr12 - 483.9983 * contr13 + 469.6086 * contr14 + 195.077 * contr23 - 1507.3809 * contr24 + 160.6044 * contr34 + 3266.6525 ;
}

double DistanceEstimator::LM149()
{
	return 1772.8823 * diff12 - 1693.4145 * diff13 - 1074.2146 * diff14 - 414.7289 * diff23 + 7.0596 * diff24 - 41.6018 * diff34 - 20.3145 * contr12 - 24.7407 * contr13 - 3.5521 * contr14 + 1117.5446 * contr23 - 22.7545 * contr24 + 1363.306 * contr34 + 3046.7549 ;
}

double DistanceEstimator::LM150()
{
	return -9.6198 * diff12 - 40.3192 * diff13 + 53.491 * diff14 - 484.7293 * diff23 + 7.0596 * diff24 - 41.6018 * diff34 - 20.3145 * contr12 - 24.7407 * contr13 - 3.5521 * contr14 + 293.2987 * contr23 - 22.7545 * contr24 + 250.729 * contr34 + 2791.057 ;
}

double DistanceEstimator::LM151()
{
	return -45.1836 * diff12 + 390.408 * diff13 + 53.491 * diff14 - 1650.4031 * diff23 + 7.0596 * diff24 - 41.6018 * diff34 - 717.6187 * contr12 - 24.7407 * contr13 - 3.5521 * contr14 + 778.0831 * contr23 - 22.7545 * contr24 + 162.5818 * contr34 + 3003.5667 ;
}

double DistanceEstimator::LM152()
{
	return -831.8238 * diff12 - 5.8126 * diff13 + 92.0865 * diff14 - 80.9418 * diff23 + 7.0596 * diff24 - 12.9061 * diff34 - 815.4164 * contr12 - 24.7407 * contr13 - 3.5521 * contr14 + 94.9682 * contr23 - 22.7545 * contr24 + 61.0281 * contr34 + 2799.2942 ;
}

double DistanceEstimator::LM153()
{
	return -118.5761 * diff12 + 353.5892 * diff13 + 1291.811 * diff14 - 1074.0198 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 + 397.6867 * contr12 + 10.9087 * contr13 - 245.9371 * contr14 + 396.9358 * contr23 - 79.664 * contr24 + 120.7433 * contr34 + 2661.3252 ;
}

double DistanceEstimator::LM154()
{
	return -118.5761 * diff12 + 781.0964 * diff13 + 7.3413 * diff14 - 3258.8123 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 + 773.673 * contr12 + 10.9087 * contr13 - 347.6408 * contr14 + 396.9358 * contr23 - 79.664 * contr24 + 120.7433 * contr34 + 3286.1882 ;
}

double DistanceEstimator::LM155()
{
	return -118.5761 * diff12 + 617.3527 * diff13 + 7.3413 * diff14 - 2867.6312 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 + 927.9853 * contr12 + 10.9087 * contr13 - 347.6408 * contr14 + 396.9358 * contr23 - 79.664 * contr24 + 120.7433 * contr34 + 3235.0115 ;
}

double DistanceEstimator::LM156()
{
	return 215.0562 * diff12 + 671.4565 * diff13 + 7.3413 * diff14 - 743.4089 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 + 1729.9539 * contr12 + 10.9087 * contr13 - 218.3625 * contr14 + 301.7603 * contr23 - 79.664 * contr24 + 120.7433 * contr34 + 2750.7672 ;
}

double DistanceEstimator::LM157()
{
	return 396.8775 * diff12 + 743.3822 * diff13 + 7.3413 * diff14 - 743.4089 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 + 1248.0539 * contr12 + 10.9087 * contr13 - 218.3625 * contr14 + 301.7603 * contr23 - 79.664 * contr24 + 120.7433 * contr34 + 2789.8232 ;
}

double DistanceEstimator::LM158()
{
	return 28.8143 * diff12 + 319.4553 * diff13 + 3231.1542 * diff14 - 743.4089 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 + 157.2994 * contr12 + 10.9087 * contr13 - 218.3625 * contr14 + 368.1883 * contr23 - 79.664 * contr24 + 120.7433 * contr34 + 2124.4083 ;
}

double DistanceEstimator::LM159()
{
	return -230.5692 * diff12 + 9.5065 * diff13 + 219.6464 * diff14 - 2364.8221 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 - 49.1369 * contr12 - 203.0487 * contr13 - 193.5289 * contr14 + 403.2426 * contr23 - 601.6418 * contr24 + 130.9114 * contr34 + 3388.6313 ;
}

double DistanceEstimator::LM160()
{
	return -64.8675 * diff12 + 9.5065 * diff13 + 1231.4299 * diff14 - 776.0653 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 - 49.1369 * contr12 - 203.0487 * contr13 - 193.5289 * contr14 + 403.2426 * contr23 - 549.7961 * contr24 + 110.3771 * contr34 + 2782.5262 ;
}

double DistanceEstimator::LM161()
{
	return 339.4695 * diff12 + 9.5065 * diff13 + 7.3413 * diff14 - 2305.2445 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 - 36.5871 * contr12 + 774.3503 * contr13 - 881.5848 * contr14 + 1595.7166 * contr23 - 369.4259 * contr24 + 1219.6245 * contr34 + 3202.1332 ;
}

double DistanceEstimator::LM162()
{
	return 74.0195 * diff12 + 9.5065 * diff13 + 7.3413 * diff14 - 2016.7455 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 - 36.5871 * contr12 + 774.3503 * contr13 - 881.5848 * contr14 + 1595.7166 * contr23 - 369.4259 * contr24 + 1219.6245 * contr34 + 3141.7697 ;
}

double DistanceEstimator::LM163()
{
	return 74.0195 * diff12 + 9.5065 * diff13 + 7.3413 * diff14 - 2016.7455 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 - 36.5871 * contr12 + 774.3503 * contr13 - 881.5848 * contr14 + 1595.7166 * contr23 - 369.4259 * contr24 + 1219.6245 * contr34 + 3128.176 ;
}

double DistanceEstimator::LM164()
{
	return 74.0195 * diff12 + 9.5065 * diff13 + 7.3413 * diff14 - 2016.7455 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 - 36.5871 * contr12 + 774.3503 * contr13 - 259.9713 * contr14 + 1595.7166 * contr23 - 369.4259 * contr24 + 1219.6245 * contr34 + 2967.5299 ;
}

double DistanceEstimator::LM165()
{
	return 74.0195 * diff12 + 9.5065 * diff13 + 7.3413 * diff14 - 2016.7455 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 - 36.5871 * contr12 + 774.3503 * contr13 - 161.8218 * contr14 + 1595.7166 * contr23 - 369.4259 * contr24 + 1219.6245 * contr34 + 2947.2484 ;
}

double DistanceEstimator::LM166()
{
	return 179.1835 * diff12 - 196.6303 * diff13 + 531.6918 * diff14 - 1359.8141 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 - 609.8946 * contr12 + 323.3042 * contr13 - 762.9276 * contr14 + 2345.0943 * contr23 - 369.4259 * contr24 + 668.0288 * contr34 + 2916.2853 ;
}

double DistanceEstimator::LM167()
{
	return 179.1835 * diff12 - 196.6303 * diff13 + 432.4903 * diff14 - 1359.8141 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 - 609.8946 * contr12 + 323.3042 * contr13 - 1033.2025 * contr14 + 2345.0943 * contr23 - 369.4259 * contr24 + 668.0288 * contr34 + 3030.3204 ;
}

double DistanceEstimator::LM168()
{
	return 184.5355 * diff12 - 402.9495 * diff13 + 7.3413 * diff14 - 1947.3314 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 - 627.3129 * contr12 + 124.0695 * contr13 - 542.3851 * contr14 + 3727.924 * contr23 - 369.4259 * contr24 + 668.0288 * contr34 + 2993.4453 ;
}

double DistanceEstimator::LM169()
{
	return 184.5355 * diff12 - 810.5629 * diff13 + 7.3413 * diff14 - 1718.5494 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 - 627.3129 * contr12 + 1206.8752 * contr13 - 542.3851 * contr14 + 3189.3323 * contr23 - 369.4259 * contr24 + 668.0288 * contr34 + 3040.0084 ;
}

double DistanceEstimator::LM170()
{
	return 220.0617 * diff12 - 786.8332 * diff13 + 7.3413 * diff14 - 2548.4004 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 + 759.4358 * contr12 + 2970.4022 * contr13 - 542.3851 * contr14 + 2881.9699 * contr23 - 369.4259 * contr24 + 668.0288 * contr34 + 3056.7827 ;
}

double DistanceEstimator::LM171()
{
	return 220.0617 * diff12 - 786.8332 * diff13 + 7.3413 * diff14 - 2548.4004 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 + 759.4358 * contr12 + 2970.4022 * contr13 - 542.3851 * contr14 + 2881.9699 * contr23 - 369.4259 * contr24 + 668.0288 * contr34 + 3055.9953 ;
}

double DistanceEstimator::LM172()
{
	return 220.0617 * diff12 - 518.7246 * diff13 + 7.3413 * diff14 - 2144.5818 * diff23 + 7.0596 * diff24 + 14.0547 * diff34 + 199.7048 * contr12 + 1999.7996 * contr13 - 542.3851 * contr14 + 2881.9699 * contr23 - 369.4259 * contr24 + 668.0288 * contr34 + 3037.1201 ;
}

double DistanceEstimator::LM173()
{
	return -119.1667 * diff12 + 9.5065 * diff13 + 602.3051 * diff14 - 1182.483 * diff23 + 7.0596 * diff24 - 215.687 * diff34 - 48.4003 * contr12 - 55.7483 * contr13 - 148.576 * contr14 + 256.9568 * contr23 - 153.8301 * contr24 + 10.8003 * contr34 + 2918.7035 ;
}

double DistanceEstimator::LM174()
{
	return -201.4912 * diff12 + 9.5065 * diff13 + 7.3413 * diff14 - 2128.5787 * diff23 + 7.0596 * diff24 - 215.687 * diff34 + 426.7034 * contr12 - 55.7483 * contr13 - 148.576 * contr14 + 256.9568 * contr23 - 153.8301 * contr24 + 10.8003 * contr34 + 3235.2555 ;
}

double DistanceEstimator::LM175()
{
	return -201.4912 * diff12 + 9.5065 * diff13 + 7.3413 * diff14 - 2076.4533 * diff23 + 7.0596 * diff24 - 477.5295 * diff34 + 273.9909 * contr12 - 55.7483 * contr13 - 148.576 * contr14 + 256.9568 * contr23 - 153.8301 * contr24 + 10.8003 * contr34 + 3238.9175 ;
}

double DistanceEstimator::LM176()
{
	return -64.8675 * diff12 + 9.5065 * diff13 + 7.3413 * diff14 - 1145.5308 * diff23 + 7.0596 * diff24 - 310.541 * diff34 - 124.3459 * contr12 - 55.7483 * contr13 - 148.576 * contr14 + 175.3659 * contr23 - 153.8301 * contr24 + 10.8003 * contr34 + 3095.9101 ;
}

double DistanceEstimator::LM177()
{
	return -64.8675 * diff12 + 9.5065 * diff13 + 7.3413 * diff14 - 1145.5308 * diff23 + 7.0596 * diff24 - 310.541 * diff34 - 124.3459 * contr12 - 55.7483 * contr13 - 148.576 * contr14 + 148.169 * contr23 - 153.8301 * contr24 + 10.8003 * contr34 + 3090.0517 ;
}

double DistanceEstimator::LM178()
{
	return -64.8675 * diff12 + 9.5065 * diff13 + 7.3413 * diff14 - 2933.5726 * diff23 + 7.0596 * diff24 - 5825.13 * diff34 - 165.0429 * contr12 - 55.7483 * contr13 - 256.5249 * contr14 + 256.9568 * contr23 - 153.8301 * contr24 - 3329.8476 * contr34 + 4527.9859 ;
}

double DistanceEstimator::LM179()
{
	return -64.8675 * diff12 + 9.5065 * diff13 + 7.3413 * diff14 - 2670.2743 * diff23 + 7.0596 * diff24 - 265.5196 * diff34 - 165.0429 * contr12 - 55.7483 * contr13 - 256.5249 * contr14 + 256.9568 * contr23 - 153.8301 * contr24 - 2838.3188 * contr34 + 4000.7326 ;
}

double DistanceEstimator::LM180()
{
	return -64.8675 * diff12 + 9.5065 * diff13 + 7.3413 * diff14 - 1890.9454 * diff23 + 7.0596 * diff24 - 265.5196 * diff34 - 165.0429 * contr12 - 55.7483 * contr13 - 256.5249 * contr14 + 256.9568 * contr23 - 153.8301 * contr24 - 2838.3188 * contr34 + 3823.8289 ;
}

double DistanceEstimator::LM181()
{
	return 379.8636 * diff12 - 5.8126 * diff13 + 527.6665 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 3095.9653 * diff34 + 69.4858 * contr12 + 33.9638 * contr13 - 5.729 * contr14 - 2518.1214 * contr23 - 84.6426 * contr24 - 5429.9565 * contr34 + 3915.9095 ;
}

double DistanceEstimator::LM182()
{
	return 401.2551 * diff12 - 5.8126 * diff13 + 527.6665 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 3111.3079 * diff34 + 69.4858 * contr12 + 33.9638 * contr13 - 5.729 * contr14 - 2518.1214 * contr23 - 84.6426 * contr24 - 5429.9565 * contr34 + 3914.925 ;
}

double DistanceEstimator::LM183()
{
	return 96.1581 * diff12 - 5.8126 * diff13 + 527.6665 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 3144.592 * diff34 + 69.4858 * contr12 + 33.9638 * contr13 - 5.729 * contr14 - 2560.883 * contr23 - 84.6426 * contr24 - 5545.4519 * contr34 + 3961.9873 ;
}

double DistanceEstimator::LM184()
{
	return -492.9621 * diff12 - 5.8126 * diff13 + 527.6665 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 3001.5058 * diff34 + 29.5117 * contr12 + 33.9638 * contr13 - 5.729 * contr14 - 2443.7541 * contr23 - 84.6426 * contr24 - 5257.6075 * contr34 + 3933.0729 ;
}

double DistanceEstimator::LM185()
{
	return -492.9621 * diff12 - 5.8126 * diff13 + 527.6665 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 3001.5058 * diff34 + 20.106 * contr12 + 33.9638 * contr13 - 5.729 * contr14 - 2443.7541 * contr23 - 84.6426 * contr24 - 5257.6075 * contr34 + 3932.6436 ;
}

double DistanceEstimator::LM186()
{
	return -492.9621 * diff12 - 5.8126 * diff13 + 527.6665 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 2727.0976 * diff34 + 69.4858 * contr12 + 33.9638 * contr13 - 5.729 * contr14 - 2296.0941 * contr23 - 84.6426 * contr24 - 4468.2608 * contr34 + 3764.1838 ;
}

double DistanceEstimator::LM187()
{
	return -492.9621 * diff12 - 5.8126 * diff13 + 527.6665 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 2347.7658 * diff34 + 69.4858 * contr12 + 33.9638 * contr13 - 5.729 * contr14 - 1762.8455 * contr23 - 84.6426 * contr24 - 3276.2268 * contr34 + 3508.7341 ;
}

double DistanceEstimator::LM188()
{
	return -492.9621 * diff12 - 5.8126 * diff13 + 527.6665 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 2125.5946 * diff34 + 1041.3689 * contr12 + 422.1932 * contr13 - 5.729 * contr14 - 1133.2643 * contr23 - 84.6426 * contr24 - 2130.2556 * contr34 + 3141.6703 ;
}

double DistanceEstimator::LM189()
{
	return -492.9621 * diff12 - 5.8126 * diff13 + 527.6665 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 2125.5946 * diff34 + 895.5864 * contr12 + 422.1932 * contr13 - 5.729 * contr14 - 1133.2643 * contr23 - 84.6426 * contr24 - 2130.2556 * contr34 + 3161.9352 ;
}

double DistanceEstimator::LM190()
{
	return -492.9621 * diff12 - 5.8126 * diff13 + 527.6665 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 2125.5946 * diff34 + 69.4858 * contr12 + 318.6654 * contr13 - 5.729 * contr14 - 1133.2643 * contr23 - 84.6426 * contr24 - 2130.2556 * contr34 + 3262.3255 ;
}

double DistanceEstimator::LM191()
{
	return -492.9621 * diff12 - 34.3067 * diff13 + 2253.5868 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 1046.8089 * diff34 + 276.9382 * contr12 + 76.3691 * contr13 - 5.729 * contr14 + 61.0791 * contr23 - 1169.2608 * contr24 - 300.6941 * contr34 + 2701.9031 ;
}

double DistanceEstimator::LM192()
{
	return -492.9621 * diff12 - 34.3067 * diff13 + 2253.5868 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 1046.8089 * diff34 + 283.8288 * contr12 + 76.3691 * contr13 - 5.729 * contr14 + 61.0791 * contr23 - 1169.2608 * contr24 - 300.6941 * contr34 + 2703.8887 ;
}

double DistanceEstimator::LM193()
{
	return -492.9621 * diff12 - 34.3067 * diff13 + 2829.3802 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 1046.8089 * diff34 + 242.4852 * contr12 + 76.3691 * contr13 - 5.729 * contr14 + 61.0791 * contr23 - 1169.2608 * contr24 - 300.6941 * contr34 + 2565.9256 ;
}

double DistanceEstimator::LM194()
{
	return -492.9621 * diff12 - 34.3067 * diff13 + 2652.213 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 1046.8089 * diff34 + 242.4852 * contr12 + 76.3691 * contr13 - 5.729 * contr14 + 61.0791 * contr23 - 1169.2608 * contr24 - 300.6941 * contr34 + 2606.5657 ;
}

double DistanceEstimator::LM195()
{
	return -492.9621 * diff12 - 139.404 * diff13 + 2108.5881 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 1046.8089 * diff34 + 190.6002 * contr12 + 76.3691 * contr13 - 5.729 * contr14 + 61.0791 * contr23 - 928.7256 * contr24 - 300.6941 * contr34 + 2662.5426 ;
}

double DistanceEstimator::LM196()
{
	return -492.9621 * diff12 + 268.8457 * diff13 + 631.7316 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 1046.8089 * diff34 + 69.4858 * contr12 + 76.3691 * contr13 - 5.729 * contr14 + 61.0791 * contr23 - 717.0537 * contr24 - 300.6941 * contr34 + 2897.6617 ;
}

double DistanceEstimator::LM197()
{
	return -2920.0244 * diff12 - 1359.8569 * diff13 + 7.3413 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 1342.9602 * diff34 + 5050.6746 * contr12 - 5356.905 * contr13 - 5.729 * contr14 - 802.0185 * contr23 - 84.6426 * contr24 - 2932.7643 * contr34 + 4249.2279 ;
}

double DistanceEstimator::LM198()
{
	return -4107.2741 * diff12 - 3306.8534 * diff13 + 7.3413 * diff14 - 301.2976 * diff23 + 7.0596 * diff24 + 1342.9602 * diff34 + 5811.1561 * contr12 - 8696.0745 * contr13 - 5.729 * contr14 - 802.0185 * contr23 - 84.6426 * contr24 - 3456.2894 * contr34 + 5063.4072 ;
}

double DistanceEstimator::LM199()
{
	return -659.2676 * diff12 + 46.6211 * diff13 + 1148.2434 * diff14 + 97.2256 * diff23 + 780.637 * diff24 - 665.8032 * diff34 - 608.713 * contr12 + 310.0905 * contr13 + 6.568 * contr14 - 534.9158 * contr23 - 47.759 * contr24 - 68.0509 * contr34 + 1479.2148 ;
}

double DistanceEstimator::LM200()
{
	return -961.3568 * diff12 + 46.6211 * diff13 + 1148.2434 * diff14 + 97.2256 * diff23 + 780.637 * diff24 - 665.8032 * diff34 - 608.713 * contr12 + 453.2447 * contr13 + 6.568 * contr14 - 572.426 * contr23 - 47.759 * contr24 - 68.0509 * contr34 + 1492.2039 ;
}

double DistanceEstimator::LM201()
{
	return -2956.1695 * diff12 + 46.6211 * diff13 + 1148.2434 * diff14 + 97.2256 * diff23 + 780.637 * diff24 - 665.8032 * diff34 - 608.713 * contr12 + 1123.534 * contr13 + 6.568 * contr14 - 606.3061 * contr23 - 47.759 * contr24 - 68.0509 * contr34 + 1423.576 ;
}

double DistanceEstimator::LM202()
{
	return -4533.9103 * diff12 + 46.6211 * diff13 + 1190.7119 * diff14 + 97.2256 * diff23 + 780.637 * diff24 - 2751.8264 * diff34 - 1267.8175 * contr12 + 278.0429 * contr13 + 6.568 * contr14 + 323.3081 * contr23 - 47.759 * contr24 - 68.0509 * contr34 + 1804.2401 ;
}

double DistanceEstimator::LM203()
{
	return 80.1169 * diff12 + 46.6211 * diff13 - 2235.7507 * diff14 + 97.2256 * diff23 + 5063.5855 * diff24 - 740.9135 * diff34 - 976.7672 * contr12 + 278.0429 * contr13 + 6.568 * contr14 + 528.0678 * contr23 - 47.759 * contr24 - 68.0509 * contr34 + 1467.0193 ;
}

double DistanceEstimator::LM204()
{
	return 80.1169 * diff12 + 46.6211 * diff13 - 9330.492 * diff14 + 97.2256 * diff23 + 2851.7523 * diff24 - 740.9135 * diff34 - 2811.9974 * contr12 + 278.0429 * contr13 + 6.568 * contr14 + 430.9001 * contr23 - 47.759 * contr24 - 68.0509 * contr34 + 4107.9169 ;
}

double DistanceEstimator::LM205()
{
	return -2183.2649 * diff12 + 46.6211 * diff13 - 2765.4522 * diff14 + 97.2256 * diff23 + 2468.2169 * diff24 - 740.9135 * diff34 - 1273.0259 * contr12 + 278.0429 * contr13 + 6.568 * contr14 + 178.3388 * contr23 - 47.759 * contr24 - 68.0509 * contr34 + 2311.5486 ;
}

double DistanceEstimator::LM206()
{
	return 1200.1926 * diff12 + 1287.2878 * diff13 + 731.4602 * diff14 - 144.1154 * diff23 + 508.2794 * diff24 - 4.4798 * diff34 - 4575.5387 * contr12 + 651.661 * contr13 + 989.4974 * contr14 - 3090.608 * contr23 - 60.5241 * contr24 - 253.021 * contr34 + 1947.2783 ;
}

double DistanceEstimator::LM207()
{
	return 1200.1926 * diff12 + 1287.2878 * diff13 + 731.4602 * diff14 - 458.0113 * diff23 + 508.2794 * diff24 - 4.4798 * diff34 - 4575.5387 * contr12 + 651.661 * contr13 + 989.4974 * contr14 - 3090.608 * contr23 - 60.5241 * contr24 - 253.021 * contr34 + 1982.268 ;
}

double DistanceEstimator::LM208()
{
	return 1375.775 * diff12 + 1287.2878 * diff13 + 731.4602 * diff14 - 144.1154 * diff23 + 508.2794 * diff24 - 4.4798 * diff34 - 4575.5387 * contr12 + 752.4858 * contr13 + 989.4974 * contr14 - 3090.608 * contr23 - 60.5241 * contr24 - 253.021 * contr34 + 1893.3527 ;
}

double DistanceEstimator::LM209()
{
	return 4008.4917 * diff12 + 1952.3203 * diff13 + 731.4602 * diff14 - 386.0217 * diff23 + 508.2794 * diff24 - 4.4798 * diff34 - 5928.0498 * contr12 + 1626.3004 * contr13 + 836.5783 * contr14 - 3090.608 * contr23 - 60.5241 * contr24 - 253.021 * contr34 + 1733.0781 ;
}

double DistanceEstimator::LM210()
{
	return 3883.3232 * diff12 + 1752.1343 * diff13 + 731.4602 * diff14 - 386.0217 * diff23 + 508.2794 * diff24 - 4.4798 * diff34 - 5928.0498 * contr12 + 1626.3004 * contr13 + 836.5783 * contr14 - 3090.608 * contr23 - 60.5241 * contr24 - 253.021 * contr34 + 1795.443 ;
}

double DistanceEstimator::LM211()
{
	return 3039.7183 * diff12 + 1952.3203 * diff13 + 731.4602 * diff14 - 335.2023 * diff23 + 508.2794 * diff24 - 4.4798 * diff34 - 5853.9081 * contr12 + 1626.3004 * contr13 + 1338.9375 * contr14 - 3090.608 * contr23 - 60.5241 * contr24 - 253.021 * contr34 + 1539.1997 ;
}

double DistanceEstimator::LM212()
{
	return 3039.7183 * diff12 + 1952.3203 * diff13 + 731.4602 * diff14 - 280.1439 * diff23 + 508.2794 * diff24 - 4.4798 * diff34 - 5853.9081 * contr12 + 1626.3004 * contr13 + 1315.0157 * contr14 - 3090.608 * contr23 - 60.5241 * contr24 - 253.021 * contr34 + 1535.3234 ;
}

double DistanceEstimator::LM213()
{
	return 1547.361 * diff12 + 1854.9984 * diff13 + 731.4602 * diff14 - 512.7269 * diff23 + 508.2794 * diff24 - 4.4798 * diff34 - 4819.9521 * contr12 + 1626.3004 * contr13 + 836.5783 * contr14 - 3090.608 * contr23 - 60.5241 * contr24 - 253.021 * contr34 + 1706.9447 ;
}

double DistanceEstimator::LM214()
{
	return 1456.7074 * diff12 + 1707.6556 * diff13 + 731.4602 * diff14 - 495.7836 * diff23 + 508.2794 * diff24 - 276.2071 * diff34 - 4257.1974 * contr12 + 3120.6923 * contr13 + 1055.9393 * contr14 - 4834.7271 * contr23 - 60.5241 * contr24 - 253.021 * contr34 + 1293.8099 ;
}

double DistanceEstimator::LM215()
{
	return 1456.7074 * diff12 + 1707.6556 * diff13 + 731.4602 * diff14 - 530.8398 * diff23 + 508.2794 * diff24 - 217.9798 * diff34 - 4257.1974 * contr12 + 3120.6923 * contr13 + 1055.9393 * contr14 - 4834.7271 * contr23 - 60.5241 * contr24 - 253.021 * contr34 + 1265.5173 ;
}

double DistanceEstimator::LM216()
{
	return 705.5358 * diff12 + 1263.3173 * diff13 + 1184.2807 * diff14 + 97.2256 * diff23 + 570.1809 * diff24 - 4.4798 * diff34 - 3863.0207 * contr12 - 215.9241 * contr13 - 5043.3099 * contr14 - 2422.8103 * contr23 - 60.5241 * contr24 - 47.84 * contr34 + 3803.9452 ;
}

double DistanceEstimator::LM217()
{
	return 736.5969 * diff12 + 1263.3173 * diff13 + 1184.2807 * diff14 + 97.2256 * diff23 + 570.1809 * diff24 - 4.4798 * diff34 - 3863.0207 * contr12 - 44.4738 * contr13 - 5043.3099 * contr14 - 2422.8103 * contr23 - 60.5241 * contr24 - 47.84 * contr34 + 3767.2456 ;
}

double DistanceEstimator::LM218()
{
	return 466.849 * diff12 + 1263.3173 * diff13 + 1184.2807 * diff14 + 97.2256 * diff23 + 570.1809 * diff24 - 4.4798 * diff34 - 3863.0207 * contr12 - 303.1936 * contr13 - 5043.3099 * contr14 - 2422.8103 * contr23 - 60.5241 * contr24 - 47.84 * contr34 + 3841.1807 ;
}

double DistanceEstimator::LM219()
{
	return 474.3234 * diff12 + 1263.3173 * diff13 + 1184.2807 * diff14 + 97.2256 * diff23 + 570.1809 * diff24 - 4.4798 * diff34 - 3863.0207 * contr12 - 303.1936 * contr13 - 5043.3099 * contr14 - 2422.8103 * contr23 - 60.5241 * contr24 - 47.84 * contr34 + 3838.8642 ;
}

double DistanceEstimator::LM220()
{
	return 721.7331 * diff12 + 1263.3173 * diff13 + 1184.2807 * diff14 + 97.2256 * diff23 + 570.1809 * diff24 - 4.4798 * diff34 - 3863.0207 * contr12 + 760.6797 * contr13 + 14.583 * contr14 - 2422.8103 * contr23 - 60.5241 * contr24 - 74.8375 * contr34 + 2060.8806 ;
}

double DistanceEstimator::LM221()
{
	return 813.7726 * diff12 + 1263.3173 * diff13 + 1355.9105 * diff14 + 97.2256 * diff23 + 570.1809 * diff24 - 4.4798 * diff34 - 6698.4696 * contr12 + 957.6448 * contr13 + 36.0598 * contr14 - 2422.8103 * contr23 - 60.5241 * contr24 - 253.021 * contr34 + 2597.8854 ;
}

double DistanceEstimator::LM222()
{
	return 813.7726 * diff12 + 1263.3173 * diff13 + 1355.9105 * diff14 + 97.2256 * diff23 + 570.1809 * diff24 - 4.4798 * diff34 - 7181.1321 * contr12 + 957.6448 * contr13 + 36.0598 * contr14 - 2422.8103 * contr23 - 60.5241 * contr24 - 253.021 * contr34 + 2553.5547 ;
}

double DistanceEstimator::LM223()
{
	return 813.7726 * diff12 + 1263.3173 * diff13 + 608.5083 * diff14 + 97.2256 * diff23 + 570.1809 * diff24 - 4.4798 * diff34 - 7181.1321 * contr12 + 957.6448 * contr13 + 36.0598 * contr14 - 2422.8103 * contr23 - 60.5241 * contr24 - 253.021 * contr34 + 2733.8092 ;
}

double DistanceEstimator::LM224()
{
	return 1047.1623 * diff12 + 629.7726 * diff13 + 229.386 * diff14 + 97.2256 * diff23 + 354.9954 * diff24 - 4.4798 * diff34 + 989.3201 * contr12 + 173.8806 * contr13 + 3989.4661 * contr14 - 613.4698 * contr23 - 60.5241 * contr24 - 244.2842 * contr34 + 36.6579 ;
}

double DistanceEstimator::LM225()
{
	return 1102.0043 * diff12 + 629.7726 * diff13 + 229.386 * diff14 + 97.2256 * diff23 + 354.9954 * diff24 - 4.4798 * diff34 + 989.3201 * contr12 + 173.8806 * contr13 + 3989.4661 * contr14 - 613.4698 * contr23 - 60.5241 * contr24 - 244.2842 * contr34 + 35.3111 ;
}

double DistanceEstimator::LM226()
{
	return 618.7721 * diff12 + 774.6087 * diff13 + 229.386 * diff14 + 97.2256 * diff23 + 354.9954 * diff24 - 4.4798 * diff34 + 423.8582 * contr12 + 173.8806 * contr13 + 3989.4661 * contr14 - 613.4698 * contr23 - 60.5241 * contr24 - 244.2842 * contr34 + 148.1977 ;
}

double DistanceEstimator::LM227()
{
	return 618.7721 * diff12 + 733.8736 * diff13 + 229.386 * diff14 + 97.2256 * diff23 + 354.9954 * diff24 - 4.4798 * diff34 + 423.8582 * contr12 + 173.8806 * contr13 + 3989.4661 * contr14 - 613.4698 * contr23 - 60.5241 * contr24 - 244.2842 * contr34 + 161.4845 ;
}

double DistanceEstimator::LM228()
{
	return 354.5266 * diff12 + 726.4229 * diff13 + 229.386 * diff14 + 97.2256 * diff23 + 354.9954 * diff24 - 4.4798 * diff34 - 341.1785 * contr12 + 378.7224 * contr13 + 5980.9152 * contr14 - 613.4698 * contr23 + 1171.7543 * contr24 - 244.2842 * contr34 - 507.7386 ;
}

double DistanceEstimator::LM229()
{
	return 222.3925 * diff12 + 392.7034 * diff13 + 210.715 * diff14 + 1224.5848 * diff23 + 354.9954 * diff24 - 4.4798 * diff34 - 492.1313 * contr12 - 235.8029 * contr13 - 115.8794 * contr14 - 613.4698 * contr23 - 60.5241 * contr24 - 244.2842 * contr34 + 1600.8218 ;
}

double DistanceEstimator::LM230()
{
	return 40.3742 * diff12 + 330.2282 * diff13 + 126.7938 * diff14 + 197.4457 * diff23 + 354.9954 * diff24 - 4.4798 * diff34 - 492.1313 * contr12 - 235.8029 * contr13 + 6.568 * contr14 - 613.4698 * contr23 - 125.821 * contr24 - 244.2842 * contr34 + 1811.5696 ;
}

double DistanceEstimator::LM231()
{
	return 65.4459 * diff12 + 330.2282 * diff13 + 73.9228 * diff14 + 259.0863 * diff23 + 354.9954 * diff24 - 4.4798 * diff34 - 492.1313 * contr12 - 235.8029 * contr13 + 6.568 * contr14 - 613.4698 * contr23 - 125.821 * contr24 - 244.2842 * contr34 + 1802.5147 ;
}

double DistanceEstimator::LM232()
{
	return -2089.4142 * diff12 + 250.5185 * diff13 + 107.2888 * diff14 + 138.195 * diff23 + 354.9954 * diff24 - 4.4798 * diff34 - 492.1313 * contr12 - 235.8029 * contr13 + 6.568 * contr14 - 613.4698 * contr23 - 209.1309 * contr24 - 244.2842 * contr34 + 1996.9824 ;
}

double DistanceEstimator::LM233()
{
	return -3123.4959 * diff12 + 4167.1825 * diff13 + 453.9557 * diff14 + 684.5311 * diff23 + 409.6712 * diff24 - 2441.5154 * diff34 + 4219.4183 * contr12 - 10291.6296 * contr13 + 2411.7969 * contr14 - 3695.5365 * contr23 + 677.3058 * contr24 - 1293.282 * contr34 + 3182.5294 ;
}

double DistanceEstimator::LM234()
{
	return -646.9795 * diff12 + 281.7003 * diff13 + 472.435 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 388.6943 * diff34 - 268.3113 * contr12 - 1073.4934 * contr13 + 6.568 * contr14 - 1606.5346 * contr23 - 115.1865 * contr24 - 164.8201 * contr34 + 2893.8716 ;
}

double DistanceEstimator::LM235()
{
	return -646.9795 * diff12 + 281.7003 * diff13 + 459.817 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 388.6943 * diff34 - 268.3113 * contr12 - 1073.4934 * contr13 + 6.568 * contr14 - 1599.4947 * contr23 - 115.1865 * contr24 - 164.8201 * contr34 + 2895.5395 ;
}

double DistanceEstimator::LM236()
{
	return -646.9795 * diff12 + 307.156 * diff13 + 459.817 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 388.6943 * diff34 - 268.3113 * contr12 - 1073.4934 * contr13 + 6.568 * contr14 - 1593.1865 * contr23 - 115.1865 * contr24 - 164.8201 * contr34 + 2891.0999 ;
}

double DistanceEstimator::LM237()
{
	return -646.9795 * diff12 + 214.879 * diff13 + 485.7959 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 388.6943 * diff34 - 268.3113 * contr12 - 1073.4934 * contr13 + 6.568 * contr14 - 1495.9597 * contr23 - 115.1865 * contr24 - 120.4636 * contr34 + 2893.5899 ;
}

double DistanceEstimator::LM238()
{
	return -646.9795 * diff12 + 214.879 * diff13 + 485.7959 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 388.6943 * diff34 - 268.3113 * contr12 - 1073.4934 * contr13 + 6.568 * contr14 - 1495.9597 * contr23 - 115.1865 * contr24 - 120.4636 * contr34 + 2892.9281 ;
}

double DistanceEstimator::LM239()
{
	return -646.9795 * diff12 + 214.879 * diff13 + 415.8527 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 548.9816 * diff34 - 268.3113 * contr12 - 1073.4934 * contr13 + 6.568 * contr14 - 1601.7191 * contr23 - 115.1865 * contr24 - 198.559 * contr34 + 2929.2548 ;
}

double DistanceEstimator::LM240()
{
	return -646.9795 * diff12 + 214.879 * diff13 + 439.6868 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 192.3614 * diff34 - 268.3113 * contr12 - 1073.4934 * contr13 + 6.568 * contr14 - 1614.6141 * contr23 - 115.1865 * contr24 - 272.5438 * contr34 + 2965.4624 ;
}

double DistanceEstimator::LM241()
{
	return -646.9795 * diff12 + 282.3465 * diff13 + 355.0908 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 396.7618 * diff34 - 589.9788 * contr12 - 1200.713 * contr13 + 6.568 * contr14 - 1544.4081 * contr23 - 115.1865 * contr24 - 342.2313 * contr34 + 2910.3742 ;
}

double DistanceEstimator::LM242()
{
	return -646.9795 * diff12 + 282.3465 * diff13 + 355.0908 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 396.7618 * diff34 - 387.2325 * contr12 - 1200.713 * contr13 + 6.568 * contr14 - 1544.4081 * contr23 - 115.1865 * contr24 - 342.2313 * contr34 + 2873.357 ;
}

double DistanceEstimator::LM243()
{
	return -646.9795 * diff12 + 282.3465 * diff13 + 2073.9194 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 396.7618 * diff34 + 446.6281 * contr12 - 1200.713 * contr13 + 6.568 * contr14 - 1544.4081 * contr23 - 115.1865 * contr24 - 342.2313 * contr34 + 2313.5164 ;
}

double DistanceEstimator::LM244()
{
	return -646.9795 * diff12 + 282.3465 * diff13 + 1253.2307 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 396.7618 * diff34 + 387.0666 * contr12 - 1200.713 * contr13 + 6.568 * contr14 - 1544.4081 * contr23 - 115.1865 * contr24 - 342.2313 * contr34 + 2551.3239 ;
}

double DistanceEstimator::LM245()
{
	return -136.2484 * diff12 - 664.1738 * diff13 + 1237.6991 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 4.4798 * diff34 - 556.5252 * contr12 - 2795.8829 * contr13 + 6.568 * contr14 - 1901.3543 * contr23 + 632.5166 * contr24 - 373.6518 * contr34 + 2695.8582 ;
}

double DistanceEstimator::LM246()
{
	return -3792.9229 * diff12 - 5226.3636 * diff13 + 2475.8701 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 4.4798 * diff34 - 556.5252 * contr12 - 3889.3 * contr13 + 6.568 * contr14 - 2682.2098 * contr23 - 454.0542 * contr24 - 373.6518 * contr34 + 4452.7705 ;
}

double DistanceEstimator::LM247()
{
	return -3792.9229 * diff12 - 5226.3636 * diff13 + 2390.7975 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 4.4798 * diff34 - 556.5252 * contr12 - 3889.3 * contr13 + 6.568 * contr14 - 2682.2098 * contr23 - 454.0542 * contr24 - 373.6518 * contr34 + 4487.4219 ;
}

double DistanceEstimator::LM248()
{
	return 2353.8846 * diff12 + 1375.7137 * diff13 + 1691.9857 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 + 8881.5563 * diff34 - 1448.9782 * contr12 - 2662.3386 * contr13 - 629.977 * contr14 - 2340.5855 * contr23 - 454.0542 * contr24 - 373.6518 * contr34 + 2320.2242 ;
}

double DistanceEstimator::LM249()
{
	return 34438.2019 * diff12 + 1375.7137 * diff13 + 1691.9857 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 + 11387.6353 * diff34 - 1448.9782 * contr12 - 2300.8707 * contr13 - 946.7271 * contr14 - 2340.5855 * contr23 - 454.0542 * contr24 - 373.6518 * contr34 + 729.9495 ;
}

double DistanceEstimator::LM250()
{
	return 37856.199 * diff12 + 1375.7137 * diff13 + 1691.9857 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 + 11387.6353 * diff34 - 1448.9782 * contr12 - 2300.8707 * contr13 - 946.7271 * contr14 - 2340.5855 * contr23 - 454.0542 * contr24 - 373.6518 * contr34 + 594.4076 ;
}

double DistanceEstimator::LM251()
{
	return -2385.674 * diff12 + 1712.0706 * diff13 + 1691.9857 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 + 8067.2686 * diff34 - 1519.4351 * contr12 - 3204.5405 * contr13 + 6.568 * contr14 - 2340.5855 * contr23 - 454.0542 * contr24 - 373.6518 * contr34 + 2221.038 ;
}

double DistanceEstimator::LM252()
{
	return -2459.0895 * diff12 + 1712.0706 * diff13 + 1691.9857 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 + 8067.2686 * diff34 - 1519.4351 * contr12 - 3204.5405 * contr13 + 6.568 * contr14 - 2340.5855 * contr23 - 454.0542 * contr24 - 373.6518 * contr34 + 2221.1137 ;
}

double DistanceEstimator::LM253()
{
	return 430.2085 * diff12 + 125.7651 * diff13 + 1557.131 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 4.4798 * diff34 - 556.5252 * contr12 - 4262.3755 * contr13 - 960.011 * contr14 - 2473.9557 * contr23 - 1124.086 * contr24 - 373.6518 * contr34 + 3162.5374 ;
}

double DistanceEstimator::LM254()
{
	return 430.2085 * diff12 + 125.7651 * diff13 + 1557.131 * diff14 + 343.1837 * diff23 + 409.6712 * diff24 - 4.4798 * diff34 - 556.5252 * contr12 - 3950.0833 * contr13 - 242.2672 * contr14 - 2473.9557 * contr23 - 1124.086 * contr24 - 373.6518 * contr34 + 2920.8188 ;
}

double DistanceEstimator::LM255()
{
	return -1522.0089 * diff12 + 4835.5293 * diff13 + 5697.9416 * diff14 + 21.3361 * diff23 + 454.8799 * diff24 - 1013.4217 * diff34 - 1084.385 * contr12 + 7.6768 * contr13 + 441.9054 * contr14 - 359.5355 * contr23 + 3603.7931 * contr24 - 3154.758 * contr34 - 171.9799 ;
}

double DistanceEstimator::LM256()
{
	return -195.1538 * diff12 + 1008.3474 * diff13 + 4778.2121 * diff14 + 21.3361 * diff23 + 454.8799 * diff24 - 1415.674 * diff34 + 1170.3238 * contr12 + 7.6768 * contr13 + 441.9054 * contr14 - 1004.3469 * contr23 + 2436.3113 * contr24 - 413.5783 * contr34 + 682.2237 ;
}

double DistanceEstimator::LM257()
{
	return -114.7843 * diff12 + 1008.3474 * diff13 + 4778.2121 * diff14 + 21.3361 * diff23 + 454.8799 * diff24 - 1415.674 * diff34 + 1170.3238 * contr12 + 7.6768 * contr13 + 441.9054 * contr14 - 1127.1701 * contr23 + 2436.3113 * contr24 - 413.5783 * contr34 + 672.7587 ;
}

double DistanceEstimator::LM258()
{
	return 217.1879 * diff12 + 1008.3474 * diff13 + 4778.2121 * diff14 + 21.3361 * diff23 + 454.8799 * diff24 - 1415.674 * diff34 + 1170.3238 * contr12 + 7.6768 * contr13 + 441.9054 * contr14 - 1127.1701 * contr23 + 2436.3113 * contr24 - 413.5783 * contr34 + 631.5597 ;
}

double DistanceEstimator::LM259()
{
	return -802.2461 * diff12 + 1008.3474 * diff13 + 5529.7787 * diff14 + 21.3361 * diff23 + 454.8799 * diff24 - 4175.8163 * diff34 + 1170.3238 * contr12 + 7.6768 * contr13 + 441.9054 * contr14 - 1139.4358 * contr23 + 2436.3113 * contr24 - 413.5783 * contr34 + 677.6788 ;
}

double DistanceEstimator::LM260()
{
	return -743.8744 * diff12 + 1008.3474 * diff13 + 5529.7787 * diff14 + 21.3361 * diff23 + 454.8799 * diff24 - 4175.8163 * diff34 + 1170.3238 * contr12 + 7.6768 * contr13 + 441.9054 * contr14 - 1139.4358 * contr23 + 2436.3113 * contr24 - 413.5783 * contr34 + 670.2025 ;
}

double DistanceEstimator::LM261()
{
	return -210.5323 * diff12 + 1008.3474 * diff13 + 5529.7787 * diff14 + 21.3361 * diff23 + 454.8799 * diff24 - 3823.4439 * diff34 + 1170.3238 * contr12 + 7.6768 * contr13 + 441.9054 * contr14 - 1139.4358 * contr23 + 2436.3113 * contr24 - 413.5783 * contr34 + 592.093 ;
}

double DistanceEstimator::LM262()
{
	return 72.8155 * diff12 + 1306.1955 * diff13 + 4209.7038 * diff14 + 21.3361 * diff23 + 334.2788 * diff24 - 664.6265 * diff34 + 1386.5342 * contr12 - 64.9388 * contr13 + 580.2759 * contr14 - 359.5355 * contr23 + 959.4751 * contr24 - 164.8372 * contr34 + 778.1851 ;
}

double DistanceEstimator::LM263()
{
	return 72.8155 * diff12 + 1306.1955 * diff13 + 4209.7038 * diff14 + 21.3361 * diff23 + 334.2788 * diff24 - 664.6265 * diff34 + 1480.0008 * contr12 - 64.9388 * contr13 + 580.2759 * contr14 - 359.5355 * contr23 + 959.4751 * contr24 - 164.8372 * contr34 + 772.9006 ;
}

double DistanceEstimator::LM264()
{
	return 72.8155 * diff12 + 1306.1955 * diff13 + 4405.3475 * diff14 + 21.3361 * diff23 + 334.2788 * diff24 - 664.6265 * diff34 + 1157.3001 * contr12 - 64.9388 * contr13 + 580.2759 * contr14 - 359.5355 * contr23 + 959.4751 * contr24 - 164.8372 * contr34 + 756.1883 ;
}

double DistanceEstimator::LM265()
{
	return 72.8155 * diff12 + 1306.1955 * diff13 + 3947.409 * diff14 + 21.3361 * diff23 + 334.2788 * diff24 - 664.6265 * diff34 + 890.4251 * contr12 + 47.1799 * contr13 + 580.2759 * contr14 - 359.5355 * contr23 + 959.4751 * contr24 - 164.8372 * contr34 + 903.7344 ;
}

double DistanceEstimator::LM266()
{
	return 72.8155 * diff12 + 2709.5172 * diff13 + 2640.4034 * diff14 + 21.3361 * diff23 + 334.2788 * diff24 - 664.6265 * diff34 + 275.6286 * contr12 - 341.1662 * contr13 + 580.2759 * contr14 - 359.5355 * contr23 + 959.4751 * contr24 - 164.8372 * contr34 + 1112.0271 ;
}

double DistanceEstimator::LM267()
{
	return 72.8155 * diff12 + 1720.1904 * diff13 + 6015.4017 * diff14 + 21.3361 * diff23 + 334.2788 * diff24 - 664.6265 * diff34 - 639.6653 * contr12 - 342.6328 * contr13 + 580.2759 * contr14 - 359.5355 * contr23 + 959.4751 * contr24 - 164.8372 * contr34 + 467.3691 ;
}

double DistanceEstimator::LM268()
{
	return 72.8155 * diff12 + 1720.1904 * diff13 + 2682.2817 * diff14 + 21.3361 * diff23 + 334.2788 * diff24 - 664.6265 * diff34 - 163.8033 * contr12 - 342.6328 * contr13 + 580.2759 * contr14 - 359.5355 * contr23 + 959.4751 * contr24 - 164.8372 * contr34 + 1395.3503 ;
}

double DistanceEstimator::LM269()
{
	return 848.0097 * diff12 + 1069.2242 * diff13 + 2615.797 * diff14 + 21.3361 * diff23 + 334.2788 * diff24 - 664.6265 * diff34 + 264.0522 * contr12 - 150.4807 * contr13 + 610.3292 * contr14 - 359.5355 * contr23 + 1180.7165 * contr24 - 164.8372 * contr34 + 1322.5232 ;
}

double DistanceEstimator::LM270()
{
	return 72.8155 * diff12 + 823.3592 * diff13 + 3454.986 * diff14 + 21.3361 * diff23 + 334.2788 * diff24 - 664.6265 * diff34 + 264.0522 * contr12 + 7.6768 * contr13 + 610.3292 * contr14 - 359.5355 * contr23 + 595.3011 * contr24 - 164.8372 * contr34 + 1262.9726 ;
}

double DistanceEstimator::LM271()
{
	return 72.8155 * diff12 + 823.3592 * diff13 + 5561.3442 * diff14 + 21.3361 * diff23 + 334.2788 * diff24 - 664.6265 * diff34 + 264.0522 * contr12 + 7.6768 * contr13 + 4384.1311 * contr14 - 359.5355 * contr23 + 795.3982 * contr24 - 164.8372 * contr34 - 522.944 ;
}

double DistanceEstimator::LM272()
{
	return 72.8155 * diff12 + 2173.5516 * diff13 + 2953.2393 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 881.0441 * diff34 + 33.0288 * contr12 + 7.6768 * contr13 + 4243.078 * contr14 - 359.5355 * contr23 + 602.5397 * contr24 + 123.4824 * contr34 + 79.3787 ;
}

double DistanceEstimator::LM273()
{
	return 72.8155 * diff12 + 2537.3636 * diff13 + 2953.2393 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 881.0441 * diff34 + 33.0288 * contr12 + 7.6768 * contr13 + 5755.3542 * contr14 - 359.5355 * contr23 + 602.5397 * contr24 + 111.2326 * contr34 - 454.0661 ;
}

double DistanceEstimator::LM274()
{
	return 72.8155 * diff12 + 2647.0543 * diff13 + 2953.2393 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 881.0441 * diff34 + 33.0288 * contr12 + 7.6768 * contr13 + 4903.92 * contr14 - 359.5355 * contr23 + 602.5397 * contr24 + 111.2326 * contr34 - 215.3656 ;
}

double DistanceEstimator::LM275()
{
	return 72.8155 * diff12 + 2404.0057 * diff13 + 2953.2393 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 + 52.3193 * diff34 + 33.0288 * contr12 + 7.6768 * contr13 + 3924.1723 * contr14 - 359.5355 * contr23 + 602.5397 * contr24 + 171.7618 * contr34 + 90.39 ;
}

double DistanceEstimator::LM276()
{
	return 72.8155 * diff12 + 2404.0057 * diff13 + 2953.2393 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 + 98.9875 * diff34 + 33.0288 * contr12 + 7.6768 * contr13 + 3924.1723 * contr14 - 359.5355 * contr23 + 602.5397 * contr24 + 171.7618 * contr34 + 91.2326 ;
}

double DistanceEstimator::LM277()
{
	return 72.8155 * diff12 + 1380.2514 * diff13 + 3655.8725 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 881.0441 * diff34 + 33.0288 * contr12 + 7.6768 * contr13 + 4037.4801 * contr14 - 359.5355 * contr23 + 602.5397 * contr24 - 37.0847 * contr34 + 160.1748 ;
}

double DistanceEstimator::LM278()
{
	return 72.8155 * diff12 + 1380.2514 * diff13 + 3655.8725 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 881.0441 * diff34 + 33.0288 * contr12 + 7.6768 * contr13 + 3473.8015 * contr14 - 359.5355 * contr23 + 602.5397 * contr24 - 37.0847 * contr34 + 338.9389 ;
}

double DistanceEstimator::LM279()
{
	return 547.8037 * diff12 + 262.5558 * diff13 + 1380.9744 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 1074.0253 * diff34 + 174.1589 * contr12 + 7.6768 * contr13 + 517.4642 * contr14 - 359.5355 * contr23 + 563.5459 * contr24 + 129.5581 * contr34 + 2144.3499 ;
}

double DistanceEstimator::LM280()
{
	return 911.1176 * diff12 + 1009.9122 * diff13 + 1380.9744 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 987.1312 * diff34 + 185.1841 * contr12 + 7.6768 * contr13 + 517.4642 * contr14 - 359.5355 * contr23 + 371.7847 * contr24 + 419.474 * contr34 + 1883.8555 ;
}

double DistanceEstimator::LM281()
{
	return 820.8035 * diff12 + 1009.9122 * diff13 + 1380.9744 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 987.1312 * diff34 + 185.1841 * contr12 + 7.6768 * contr13 + 517.4642 * contr14 - 359.5355 * contr23 + 371.7847 * contr24 + 778.0468 * contr34 + 1863.634 ;
}

double DistanceEstimator::LM282()
{
	return 820.8035 * diff12 + 1009.9122 * diff13 + 1380.9744 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 987.1312 * diff34 + 185.1841 * contr12 + 7.6768 * contr13 + 517.4642 * contr14 - 359.5355 * contr23 + 371.7847 * contr24 + 691.9893 * contr34 + 1874.374 ;
}

double DistanceEstimator::LM283()
{
	return 820.8035 * diff12 + 1009.9122 * diff13 + 1380.9744 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 987.1312 * diff34 + 185.1841 * contr12 + 7.6768 * contr13 + 517.4642 * contr14 - 359.5355 * contr23 + 371.7847 * contr24 + 419.474 * contr34 + 1905.0309 ;
}

double DistanceEstimator::LM284()
{
	return 472.4492 * diff12 + 791.4542 * diff13 + 2532.9785 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 1989.9456 * diff34 + 185.1841 * contr12 + 7.6768 * contr13 + 517.4642 * contr14 - 359.5355 * contr23 + 427.838 * contr24 + 311.3048 * contr34 + 1701.8979 ;
}

double DistanceEstimator::LM285()
{
	return 472.4492 * diff12 + 791.4542 * diff13 + 2072.1768 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 1588.8199 * diff34 + 185.1841 * contr12 + 7.6768 * contr13 + 517.4642 * contr14 - 359.5355 * contr23 + 427.838 * contr24 + 311.3048 * contr34 + 1811.3999 ;
}

double DistanceEstimator::LM286()
{
	return 472.4492 * diff12 + 791.4542 * diff13 + 1465.3348 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 1588.8199 * diff34 + 185.1841 * contr12 + 7.6768 * contr13 + 517.4642 * contr14 - 359.5355 * contr23 + 827.8315 * contr24 + 311.3048 * contr34 + 1908.6418 ;
}

double DistanceEstimator::LM287()
{
	return 472.4492 * diff12 + 791.4542 * diff13 + 400.7943 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 1588.8199 * diff34 + 185.1841 * contr12 + 7.6768 * contr13 + 517.4642 * contr14 - 359.5355 * contr23 + 927.1836 * contr24 + 311.3048 * contr34 + 2209.8773 ;
}

double DistanceEstimator::LM288()
{
	return 472.4492 * diff12 + 791.4542 * diff13 + 357.8074 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 1588.8199 * diff34 + 185.1841 * contr12 + 7.6768 * contr13 + 517.4642 * contr14 - 359.5355 * contr23 + 927.1836 * contr24 + 311.3048 * contr34 + 2222.4025 ;
}

double DistanceEstimator::LM289()
{
	return 472.4492 * diff12 + 791.4542 * diff13 + 2072.1768 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 1588.8199 * diff34 + 185.1841 * contr12 + 7.6768 * contr13 + 517.4642 * contr14 - 359.5355 * contr23 + 803.8721 * contr24 + 311.3048 * contr34 + 1734.9853 ;
}

double DistanceEstimator::LM290()
{
	return 1720.1771 * diff12 + 262.5558 * diff13 + 1380.9744 * diff14 + 21.3361 * diff23 + 222.2635 * diff24 - 1100.9153 * diff34 + 995.8182 * contr12 + 7.6768 * contr13 + 517.4642 * contr14 - 359.5355 * contr23 + 563.5459 * contr24 + 112.7278 * contr34 + 1860.0533 ;
}

double DistanceEstimator::LM291()
{
	return -3010.5178 * diff12 + 6713.5653 * diff13 - 458.4564 * diff14 + 21.3361 * diff23 + 100.683 * diff24 - 4.4798 * diff34 - 2259.2921 * contr12 - 1333.3249 * contr13 + 340.6177 * contr14 - 676.6248 * contr23 + 423.8872 * contr24 - 1767.8412 * contr34 + 2107.9518 ;
}

double DistanceEstimator::LM292()
{
	return -3053.0741 * diff12 + 6713.5653 * diff13 - 458.4564 * diff14 + 21.3361 * diff23 + 100.683 * diff24 - 4.4798 * diff34 - 2259.2921 * contr12 - 1333.3249 * contr13 + 340.6177 * contr14 - 676.6248 * contr23 + 423.8872 * contr24 - 1767.8412 * contr34 + 2095.8485 ;
}

double DistanceEstimator::LM293()
{
	return -1092.8393 * diff12 + 5613.7902 * diff13 - 458.4564 * diff14 + 21.3361 * diff23 + 100.683 * diff24 - 4.4798 * diff34 - 2259.2921 * contr12 - 1333.3249 * contr13 + 340.6177 * contr14 - 676.6248 * contr23 + 423.8872 * contr24 - 1767.8412 * contr34 + 2226.3947 ;
}

double DistanceEstimator::LM294()
{
	return 212.0576 * diff12 + 4102.5031 * diff13 - 458.4564 * diff14 + 21.3361 * diff23 + 100.683 * diff24 - 4.4798 * diff34 - 3213.2858 * contr12 - 1333.3249 * contr13 + 340.6177 * contr14 - 676.6248 * contr23 + 423.8872 * contr24 + 1570.3872 * contr34 + 2405.8883 ;
}

double DistanceEstimator::LM295()
{
	return 710.0609 * diff12 + 4102.5031 * diff13 - 458.4564 * diff14 + 21.3361 * diff23 + 100.683 * diff24 - 4.4798 * diff34 - 2857.2044 * contr12 - 1333.3249 * contr13 + 340.6177 * contr14 - 676.6248 * contr23 + 423.8872 * contr24 + 3362.9529 * contr34 + 2172.0301 ;
}

double DistanceEstimator::LM296()
{
	return -110.6189 * diff12 + 6835.7079 * diff13 - 458.4564 * diff14 + 21.3361 * diff23 + 100.683 * diff24 - 4.4798 * diff34 - 2007.2171 * contr12 - 1333.3249 * contr13 + 340.6177 * contr14 - 676.6248 * contr23 + 423.8872 * contr24 + 1048.6867 * contr34 + 1645.1851 ;
}

double DistanceEstimator::LM297()
{
	return 136.3109 * diff12 + 5008.474 * diff13 - 458.4564 * diff14 + 21.3361 * diff23 + 100.683 * diff24 - 4.4798 * diff34 - 2007.2171 * contr12 - 1333.3249 * contr13 + 340.6177 * contr14 - 676.6248 * contr23 + 423.8872 * contr24 + 1470.154 * contr34 + 2066.1008 ;
}

double DistanceEstimator::LM298()
{
	return 95.1559 * diff12 + 5008.474 * diff13 - 458.4564 * diff14 + 21.3361 * diff23 + 100.683 * diff24 - 4.4798 * diff34 - 2007.2171 * contr12 - 1333.3249 * contr13 + 340.6177 * contr14 - 676.6248 * contr23 + 423.8872 * contr24 + 1470.154 * contr34 + 2075.8749 ;
}

double DistanceEstimator::LM299()
{
	return -181.7026 * diff12 + 1350.9193 * diff13 - 458.4564 * diff14 + 21.3361 * diff23 + 100.683 * diff24 - 4.4798 * diff34 - 1900.8567 * contr12 - 3100.3157 * contr13 + 617.0727 * contr14 - 676.6248 * contr23 + 423.8872 * contr24 - 339.104 * contr34 + 2617.5474 ;
}

double DistanceEstimator::LM300()
{
	return -181.7026 * diff12 + 1575.9025 * diff13 - 458.4564 * diff14 + 21.3361 * diff23 + 100.683 * diff24 - 4.4798 * diff34 - 1900.8567 * contr12 - 3015.289 * contr13 + 627.8732 * contr14 - 676.6248 * contr23 + 423.8872 * contr24 - 339.104 * contr34 + 2534.249 ;
}

double DistanceEstimator::LM301()
{
	return -181.7026 * diff12 + 1575.9025 * diff13 - 458.4564 * diff14 + 21.3361 * diff23 + 100.683 * diff24 - 4.4798 * diff34 - 1900.8567 * contr12 - 3038.277 * contr13 + 628.9533 * contr14 - 676.6248 * contr23 + 423.8872 * contr24 - 339.104 * contr34 + 2531.8584 ;
}

double DistanceEstimator::LM302()
{
	return -145.8539 * diff12 + 5.4038 * diff13 + 965.127 * diff14 + 246.7265 * diff23 + 2.9913 * diff24 - 144.9304 * diff34 + 1.5008 * contr12 - 1157.9345 * contr13 - 127.1097 * contr14 - 35.5833 * contr23 + 131.6593 * contr24 + 297.2749 * contr34 + 2642.0683 ;
}

double DistanceEstimator::LM303()
{
	return -185.151 * diff12 + 5.4038 * diff13 + 207.9825 * diff14 - 147.0889 * diff23 + 2.9913 * diff24 - 144.9304 * diff34 + 27.3852 * contr12 - 174.2903 * contr13 - 127.1097 * contr14 - 35.5833 * contr23 + 131.6593 * contr24 + 75.3973 * contr34 + 2811.032 ;
}

double DistanceEstimator::LM304()
{
	return -313.2146 * diff12 + 5.4038 * diff13 + 256.0934 * diff14 - 132.6575 * diff23 + 2.9913 * diff24 - 144.9304 * diff34 - 9.2526 * contr12 - 104.3484 * contr13 - 127.1097 * contr14 - 35.5833 * contr23 + 131.6593 * contr24 + 38.0298 * contr34 + 2782.7641 ;
}

double DistanceEstimator::LM305()
{
	return -210.4182 * diff12 + 5.4038 * diff13 + 256.0934 * diff14 - 132.6575 * diff23 + 2.9913 * diff24 - 144.9304 * diff34 + 3.9379 * contr12 - 598.0071 * contr13 - 127.1097 * contr14 - 35.5833 * contr23 + 131.6593 * contr24 + 38.0298 * contr34 + 2823.1686 ;
}

double DistanceEstimator::LM306()
{
	return -210.4182 * diff12 + 5.4038 * diff13 + 256.0934 * diff14 - 132.6575 * diff23 + 2.9913 * diff24 - 144.9304 * diff34 + 9.3416 * contr12 - 104.3484 * contr13 - 127.1097 * contr14 - 35.5833 * contr23 + 131.6593 * contr24 + 38.0298 * contr34 + 2754.6024 ;
}

double DistanceEstimator::LM307()
{
	return -42.5173 * diff12 + 57.5107 * diff13 + 2101.3364 * diff14 - 261.3749 * diff23 + 2.9913 * diff24 - 144.9304 * diff34 - 905.9255 * contr12 - 2090.4215 * contr13 - 891.092 * contr14 - 35.5833 * contr23 + 131.6593 * contr24 + 3.0422 * contr34 + 2922.8801 ;
}

double DistanceEstimator::LM308()
{
	return -42.5173 * diff12 + 57.5107 * diff13 + 1546.735 * diff14 - 261.3749 * diff23 + 2.9913 * diff24 - 144.9304 * diff34 - 341.6413 * contr12 - 295.3143 * contr13 - 442.0634 * contr14 - 35.5833 * contr23 + 131.6593 * contr24 + 3.0422 * contr34 + 2658.2434 ;
}

double DistanceEstimator::LM309()
{
	return -184.782 * diff12 - 286.7957 * diff13 + 75.3841 * diff14 + 229.0144 * diff23 + 2.9913 * diff24 - 194.5224 * diff34 - 52.2078 * contr12 - 17.5331 * contr13 - 47.4204 * contr14 - 35.5833 * contr23 + 135.6322 * contr24 + 4.8215 * contr34 + 2616.3103 ;
}

double DistanceEstimator::LM310()
{
	return -184.782 * diff12 - 2259.8933 * diff13 + 75.3841 * diff14 - 384.5498 * diff23 + 2.9913 * diff24 - 194.5224 * diff34 - 52.2078 * contr12 - 17.5331 * contr13 - 47.4204 * contr14 - 35.5833 * contr23 + 135.6322 * contr24 + 4.8215 * contr34 + 2845.4859 ;
}

double DistanceEstimator::LM311()
{
	return -584.8814 * diff12 - 122.3333 * diff13 + 1028.5476 * diff14 + 163.6829 * diff23 + 2.9913 * diff24 - 193.4322 * diff34 + 107.775 * contr12 - 17.5331 * contr13 - 47.4204 * contr14 - 35.5833 * contr23 + 135.6322 * contr24 + 4.8215 * contr34 + 2594.438 ;
}

double DistanceEstimator::LM312()
{
	return -1011.8796 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 215.7522 * diff23 - 310.5939 * diff24 - 46.0192 * diff34 + 661.6079 * contr12 - 851.8342 * contr13 + 557.2258 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2506.0325 ;
}

double DistanceEstimator::LM313()
{
	return -1050.9602 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 215.7522 * diff23 - 310.5939 * diff24 - 46.0192 * diff34 + 510.2859 * contr12 - 1098.7301 * contr13 + 557.2258 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2578.9354 ;
}

double DistanceEstimator::LM314()
{
	return -1062.3529 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 215.7522 * diff23 - 310.5939 * diff24 - 46.0192 * diff34 + 563.8546 * contr12 - 891.2863 * contr13 + 557.2258 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2527.2803 ;
}

double DistanceEstimator::LM315()
{
	return -1195.9455 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 215.7522 * diff23 - 310.5939 * diff24 - 46.0192 * diff34 + 592.4329 * contr12 - 891.2863 * contr13 + 557.2258 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2529.258 ;
}

double DistanceEstimator::LM316()
{
	return -1189.9625 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 215.7522 * diff23 - 310.5939 * diff24 - 46.0192 * diff34 + 592.4329 * contr12 - 891.2863 * contr13 + 557.2258 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2528.7252 ;
}

double DistanceEstimator::LM317()
{
	return -444.4761 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 215.7522 * diff23 - 310.5939 * diff24 - 46.0192 * diff34 - 915.9995 * contr12 - 17.5331 * contr13 + 557.2258 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2512.7284 ;
}

double DistanceEstimator::LM318()
{
	return -444.4761 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 215.7522 * diff23 - 310.5939 * diff24 - 46.0192 * diff34 - 1053.6472 * contr12 - 17.5331 * contr13 + 557.2258 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2520.1751 ;
}

double DistanceEstimator::LM319()
{
	return -441.0583 * diff12 + 66.2883 * diff13 + 641.8707 * diff14 + 243.9555 * diff23 - 310.5939 * diff24 - 115.9114 * diff34 - 365.4086 * contr12 - 17.5331 * contr13 + 557.2258 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2310.8341 ;
}

double DistanceEstimator::LM320()
{
	return -441.0583 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 198.0079 * diff23 - 310.5939 * diff24 - 115.9114 * diff34 - 365.4086 * contr12 - 17.5331 * contr13 + 557.2258 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2434.4092 ;
}

double DistanceEstimator::LM321()
{
	return -441.0583 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 3470.4399 * diff23 - 310.5939 * diff24 - 115.9114 * diff34 - 365.4086 * contr12 - 17.5331 * contr13 + 557.2258 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 1729.2742 ;
}

double DistanceEstimator::LM322()
{
	return -441.0583 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 3662.9359 * diff23 - 310.5939 * diff24 - 115.9114 * diff34 - 365.4086 * contr12 - 17.5331 * contr13 + 557.2258 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 1688.2771 ;
}

double DistanceEstimator::LM323()
{
	return -441.0583 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 10.6401 * diff23 - 310.5939 * diff24 - 115.9114 * diff34 - 365.4086 * contr12 - 17.5331 * contr13 - 2774.0868 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 3457.536 ;
}

double DistanceEstimator::LM324()
{
	return -441.0583 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 214.5428 * diff23 - 310.5939 * diff24 - 193.2922 * diff34 - 365.4086 * contr12 - 17.5331 * contr13 + 557.2258 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2417.3626 ;
}

double DistanceEstimator::LM325()
{
	return -1277.3848 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 297.0322 * diff23 - 310.5939 * diff24 - 46.0192 * diff34 - 560.7072 * contr12 - 17.5331 * contr13 + 557.2258 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2496.9173 ;
}

double DistanceEstimator::LM326()
{
	return -63.3126 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 37.3982 * diff23 - 1044.6519 * diff24 - 46.0192 * diff34 - 539.6202 * contr12 - 17.5331 * contr13 + 591.9632 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2733.7343 ;
}

double DistanceEstimator::LM327()
{
	return -63.3126 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 37.3982 * diff23 - 2206.3796 * diff24 + 647.106 * diff34 + 884.1122 * contr12 - 17.5331 * contr13 + 2823.2514 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2143.674 ;
}

double DistanceEstimator::LM328()
{
	return -570.3511 * diff12 + 1302.291 * diff13 + 176.4753 * diff14 + 37.3982 * diff23 - 77.5187 * diff24 - 46.0192 * diff34 - 855.5472 * contr12 - 17.5331 * contr13 + 447.7239 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2253.7943 ;
}

double DistanceEstimator::LM329()
{
	return -357.1771 * diff12 + 544.5463 * diff13 + 176.4753 * diff14 + 37.3982 * diff23 - 77.5187 * diff24 - 46.0192 * diff34 - 556.5716 * contr12 - 1079.659 * contr13 + 447.7239 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2526.7609 ;
}

double DistanceEstimator::LM330()
{
	return -357.1771 * diff12 + 598.8332 * diff13 + 176.4753 * diff14 + 37.3982 * diff23 - 77.5187 * diff24 - 46.0192 * diff34 - 556.5716 * contr12 - 1059.4602 * contr13 + 447.7239 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2528.1445 ;
}

double DistanceEstimator::LM331()
{
	return -357.1771 * diff12 + 587.9293 * diff13 + 176.4753 * diff14 + 37.3982 * diff23 - 77.5187 * diff24 - 46.0192 * diff34 - 556.5716 * contr12 - 1059.4602 * contr13 + 447.7239 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2531.9745 ;
}

double DistanceEstimator::LM332()
{
	return -980.7843 * diff12 + 599.1927 * diff13 + 176.4753 * diff14 + 37.3982 * diff23 - 77.5187 * diff24 - 46.0192 * diff34 - 556.5716 * contr12 - 1446.382 * contr13 + 447.7239 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2678.634 ;
}

double DistanceEstimator::LM333()
{
	return -992.832 * diff12 + 166.8088 * diff13 + 176.4753 * diff14 + 37.3982 * diff23 - 77.5187 * diff24 - 46.0192 * diff34 - 2081.6891 * contr12 - 17.5331 * contr13 + 447.7239 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2563.1564 ;
}

double DistanceEstimator::LM334()
{
	return -1349.1733 * diff12 + 166.8088 * diff13 + 176.4753 * diff14 + 37.3982 * diff23 - 77.5187 * diff24 - 46.0192 * diff34 - 1602.3664 * contr12 - 17.5331 * contr13 + 447.7239 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2543.1972 ;
}

double DistanceEstimator::LM335()
{
	return -271.6469 * diff12 + 66.2883 * diff13 + 993.0484 * diff14 + 37.3982 * diff23 - 77.5187 * diff24 - 1773.0762 * diff34 - 393.144 * contr12 - 899.4461 * contr13 + 447.7239 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2554.447 ;
}

double DistanceEstimator::LM336()
{
	return -722.9763 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 37.3982 * diff23 - 77.5187 * diff24 - 46.0192 * diff34 - 1793.8984 * contr12 - 17.5331 * contr13 + 447.7239 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 835.5584 * contr34 + 2638.4689 ;
}

double DistanceEstimator::LM337()
{
	return -1077.7364 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 37.3982 * diff23 - 77.5187 * diff24 - 46.0192 * diff34 - 1748.748 * contr12 - 17.5331 * contr13 + 447.7239 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 835.5584 * contr34 + 2658.314 ;
}

double DistanceEstimator::LM338()
{
	return -1355.111 * diff12 + 66.2883 * diff13 + 176.4753 * diff14 + 37.3982 * diff23 - 77.5187 * diff24 - 46.0192 * diff34 - 1693.5974 * contr12 - 17.5331 * contr13 + 447.7239 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 1034.522 * contr34 + 2680.418 ;
}

double DistanceEstimator::LM339()
{
	return -54.5136 * diff12 - 321.1967 * diff13 + 206.2042 * diff14 + 113.2634 * diff23 + 77.0696 * diff24 - 382.3943 * diff34 - 541.0353 * contr12 - 427.8152 * contr13 + 192.6787 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2396.482 ;
}

double DistanceEstimator::LM340()
{
	return -54.5136 * diff12 + 227.0619 * diff13 + 206.2042 * diff14 + 438.0322 * diff23 + 77.0696 * diff24 - 531.1443 * diff34 - 541.0353 * contr12 - 331.5245 * contr13 - 3483.6683 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 3420.8337 ;
}

double DistanceEstimator::LM341()
{
	return -54.5136 * diff12 + 227.0619 * diff13 + 206.2042 * diff14 + 151.3909 * diff23 + 77.0696 * diff24 - 665.1317 * diff34 - 541.0353 * contr12 - 331.5245 * contr13 - 3483.6683 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 3478.7821 ;
}

double DistanceEstimator::LM342()
{
	return -54.5136 * diff12 + 227.0619 * diff13 + 206.2042 * diff14 + 151.3909 * diff23 + 77.0696 * diff24 - 682.6084 * diff34 - 541.0353 * contr12 - 331.5245 * contr13 - 3483.6683 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 3478.0464 ;
}

double DistanceEstimator::LM343()
{
	return -54.5136 * diff12 + 271.3672 * diff13 - 232.8136 * diff14 - 51.9034 * diff23 + 77.0696 * diff24 - 486.7772 * diff34 - 541.0353 * contr12 - 331.5245 * contr13 - 4332.1605 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 3751.7156 ;
}

double DistanceEstimator::LM344()
{
	return -54.5136 * diff12 + 271.3672 * diff13 - 116.603 * diff14 - 51.9034 * diff23 + 77.0696 * diff24 - 486.7772 * diff34 - 541.0353 * contr12 - 331.5245 * contr13 - 7622.725 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 4742.5563 ;
}

double DistanceEstimator::LM345()
{
	return -54.5136 * diff12 + 226.1148 * diff13 - 116.603 * diff14 - 51.9034 * diff23 + 77.0696 * diff24 - 486.7772 * diff34 - 541.0353 * contr12 - 331.5245 * contr13 - 7622.725 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 4748.7601 ;
}

double DistanceEstimator::LM346()
{
	return -54.5136 * diff12 + 221.0867 * diff13 - 116.603 * diff14 - 51.9034 * diff23 + 77.0696 * diff24 - 486.7772 * diff34 - 541.0353 * contr12 - 331.5245 * contr13 - 7622.725 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 4748.4655 ;
}

double DistanceEstimator::LM347()
{
	return -54.5136 * diff12 + 271.3672 * diff13 - 116.603 * diff14 - 51.9034 * diff23 + 77.0696 * diff24 - 486.7772 * diff34 - 541.0353 * contr12 - 331.5245 * contr13 - 8051.9291 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 4867.9718 ;
}

double DistanceEstimator::LM348()
{
	return -512.4767 * diff12 + 291.4664 * diff13 + 206.2042 * diff14 - 51.9034 * diff23 + 77.0696 * diff24 - 486.7772 * diff34 - 541.0353 * contr12 - 331.5245 * contr13 - 4748.3942 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 3903.4306 ;
}

double DistanceEstimator::LM349()
{
	return -530.7953 * diff12 + 291.4664 * diff13 + 206.2042 * diff14 - 51.9034 * diff23 + 77.0696 * diff24 - 486.7772 * diff34 - 541.0353 * contr12 - 331.5245 * contr13 - 4748.3942 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 3900.0014 ;
}

double DistanceEstimator::LM350()
{
	return -262.5748 * diff12 - 294.9544 * diff13 + 206.2042 * diff14 - 237.5842 * diff23 + 77.0696 * diff24 - 815.0102 * diff34 - 844.0182 * contr12 - 17.5331 * contr13 + 3128.1632 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 1589.6982 ;
}

double DistanceEstimator::LM351()
{
	return -140.4898 * diff12 + 246.387 * diff13 + 206.2042 * diff14 - 23.5535 * diff23 + 77.0696 * diff24 - 1520.442 * diff34 - 844.2404 * contr12 - 17.5331 * contr13 + 3148.105 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 1664.7587 ;
}

double DistanceEstimator::LM352()
{
	return -140.4898 * diff12 + 1109.9219 * diff13 + 206.2042 * diff14 + 12.0149 * diff23 + 77.0696 * diff24 - 503.8039 * diff34 - 905.0064 * contr12 + 1384.0667 * contr13 + 3551.724 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 1029.2351 ;
}

double DistanceEstimator::LM353()
{
	return -175.6696 * diff12 + 775.9478 * diff13 + 206.2042 * diff14 + 639.7864 * diff23 + 77.0696 * diff24 - 503.8039 * diff34 - 905.0064 * contr12 + 761.1335 * contr13 + 3551.724 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 + 1803.4876 * contr34 + 892.0282 ;
}

double DistanceEstimator::LM354()
{
	return -242.2021 * diff12 + 775.9478 * diff13 + 206.2042 * diff14 + 616.8256 * diff23 + 77.0696 * diff24 - 503.8039 * diff34 - 905.0064 * contr12 + 761.1335 * contr13 + 3551.724 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 + 1346.4632 * contr34 + 954.3935 ;
}

double DistanceEstimator::LM355()
{
	return -285.7931 * diff12 + 775.9478 * diff13 + 206.2042 * diff14 + 563.3004 * diff23 + 77.0696 * diff24 - 503.8039 * diff34 - 905.0064 * contr12 + 761.1335 * contr13 + 3551.724 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 + 1752.2543 * contr34 + 933.18 ;
}

double DistanceEstimator::LM356()
{
	return -140.4898 * diff12 + 877.8921 * diff13 + 206.2042 * diff14 + 302.9405 * diff23 + 77.0696 * diff24 - 503.8039 * diff34 - 905.0064 * contr12 + 761.1335 * contr13 + 3551.724 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 + 1209.4292 * contr34 + 1018.2309 ;
}

double DistanceEstimator::LM357()
{
	return -529.2711 * diff12 - 129.073 * diff13 + 410.5573 * diff14 - 324.7719 * diff23 + 77.0696 * diff24 - 745.5609 * diff34 - 2668.4535 * contr12 - 1788.7189 * contr13 - 2406.521 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 3795.908 ;
}

double DistanceEstimator::LM358()
{
	return -993.3042 * diff12 - 59.833 * diff13 + 660.0972 * diff14 - 324.7719 * diff23 + 77.0696 * diff24 - 745.5609 * diff34 - 2668.4535 * contr12 - 2044.2935 * contr13 - 2873.8498 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 4001.9808 ;
}

double DistanceEstimator::LM359()
{
	return -993.3042 * diff12 - 11.7763 * diff13 + 660.0972 * diff14 - 324.7719 * diff23 + 77.0696 * diff24 - 745.5609 * diff34 - 2668.4535 * contr12 - 1942.4548 * contr13 - 2873.8498 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 3977.9065 ;
}

double DistanceEstimator::LM360()
{
	return -993.3042 * diff12 - 59.833 * diff13 + 767.0429 * diff14 - 324.7719 * diff23 + 77.0696 * diff24 - 745.5609 * diff34 - 2668.4535 * contr12 - 2498.4898 * contr13 - 2873.8498 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 4091.1755 ;
}

double DistanceEstimator::LM361()
{
	return -768.8933 * diff12 + 406.9091 * diff13 + 648.9692 * diff14 - 245.6913 * diff23 + 77.0696 * diff24 - 745.5609 * diff34 - 2668.4535 * contr12 - 1784.9917 * contr13 - 2163.7989 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 3702.6549 ;
}

double DistanceEstimator::LM362()
{
	return -2065.8074 * diff12 - 248.1645 * diff13 + 206.2042 * diff14 - 246.331 * diff23 + 77.0696 * diff24 - 1098.888 * diff34 - 2668.4535 * contr12 - 1944.9871 * contr13 + 1222.0558 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 3071.1577 ;
}

double DistanceEstimator::LM363()
{
	return -1811.7646 * diff12 - 248.1645 * diff13 + 620.0991 * diff14 - 400.6253 * diff23 + 77.0696 * diff24 - 1098.888 * diff34 - 2668.4535 * contr12 - 1944.9871 * contr13 + 2081.0202 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2699.9573 ;
}

double DistanceEstimator::LM364()
{
	return -1811.7646 * diff12 - 248.1645 * diff13 + 563.6589 * diff14 - 400.6253 * diff23 + 77.0696 * diff24 - 1098.888 * diff34 - 2668.4535 * contr12 - 1944.9871 * contr13 + 1574.9252 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 - 14.8335 * contr34 + 2872.2417 ;
}

double DistanceEstimator::LM365()
{
	return -1529.9992 * diff12 - 298.2883 * diff13 + 206.2042 * diff14 - 491.7437 * diff23 + 77.0696 * diff24 - 824.1466 * diff34 - 5854.515 * contr12 - 17.5331 * contr13 + 5101.1781 * contr14 - 35.5833 * contr23 + 91.5283 * contr24 + 6872.4206 * contr34 + 1045.6433 ;
}

double DistanceEstimator::LM366()
{
	return -84.0073 * diff12 - 121.1293 * diff13 + 54.8295 * diff14 - 15.8559 * diff23 + 127.2063 * diff24 + 21.55 * diff34 - 1230.8685 * contr12 + 34.0173 * contr13 - 563.6259 * contr14 + 103.8283 * contr23 - 84.2829 * contr24 + 64.9477 * contr34 + 2655.2566 ;
}

double DistanceEstimator::LM367()
{
	return -84.0073 * diff12 - 121.1293 * diff13 + 54.8295 * diff14 - 15.8559 * diff23 + 127.2063 * diff24 + 21.55 * diff34 - 258.5232 * contr12 + 34.0173 * contr13 - 362.7152 * contr14 + 103.8283 * contr23 - 84.2829 * contr24 + 64.9477 * contr34 + 2386.735 ;
}

double DistanceEstimator::LM368()
{
	return -84.0073 * diff12 - 121.1293 * diff13 + 54.8295 * diff14 - 15.8559 * diff23 + 127.2063 * diff24 + 21.55 * diff34 - 84.9217 * contr12 + 34.0173 * contr13 - 82.7091 * contr14 + 103.8283 * contr23 - 84.2829 * contr24 + 64.9477 * contr34 + 2318.7393 ;
}

double DistanceEstimator::LM369()
{
	return -351.2762 * diff12 - 228.3925 * diff13 + 54.8295 * diff14 - 15.8559 * diff23 + 762.8321 * diff24 + 21.55 * diff34 - 427.568 * contr12 + 34.0173 * contr13 - 49.7648 * contr14 - 72.4207 * contr23 - 1765.8683 * contr24 + 64.9477 * contr34 + 2797.0295 ;
}

double DistanceEstimator::LM370()
{
	return -966.8786 * diff12 - 228.3925 * diff13 + 54.8295 * diff14 - 15.8559 * diff23 + 762.8321 * diff24 - 663.1492 * diff34 - 459.2385 * contr12 + 34.0173 * contr13 - 49.7648 * contr14 - 72.4207 * contr23 - 1397.1412 * contr24 + 64.9477 * contr34 + 2998.2536 ;
}

double DistanceEstimator::LM371()
{
	return -1246.9399 * diff12 - 228.3925 * diff13 + 54.8295 * diff14 - 15.8559 * diff23 + 762.8321 * diff24 - 532.1659 * diff34 - 459.2385 * contr12 + 34.0173 * contr13 - 49.7648 * contr14 - 72.4207 * contr23 - 1397.1412 * contr24 + 64.9477 * contr34 + 3035.2698 ;
}

double DistanceEstimator::LM372()
{
	return -601.6039 * diff12 - 228.3925 * diff13 + 54.8295 * diff14 - 15.8559 * diff23 + 762.8321 * diff24 - 54.0541 * diff34 - 462.2483 * contr12 + 34.0173 * contr13 - 49.7648 * contr14 - 72.4207 * contr23 - 1397.1412 * contr24 + 64.9477 * contr34 + 2767.9605 ;
}

double DistanceEstimator::LM373()
{
	return -762.9818 * diff12 - 228.3925 * diff13 + 54.8295 * diff14 - 15.8559 * diff23 + 1199.7144 * diff24 + 21.55 * diff34 - 405.9895 * contr12 + 34.0173 * contr13 - 196.825 * contr14 - 72.4207 * contr23 - 1527.7368 * contr24 + 64.9477 * contr34 + 2756.711 ;
}

double DistanceEstimator::LM374()
{
	return -736.5278 * diff12 - 228.3925 * diff13 + 54.8295 * diff14 - 15.8559 * diff23 + 1199.7144 * diff24 + 21.55 * diff34 - 405.9895 * contr12 + 34.0173 * contr13 - 185.5127 * contr14 - 72.4207 * contr23 - 1527.7368 * contr24 + 64.9477 * contr34 + 2738.1206 ;
}

double DistanceEstimator::LM375()
{
	return -247.7276 * diff12 - 214.6559 * diff13 + 54.8295 * diff14 - 15.8559 * diff23 + 235.7175 * diff24 + 21.55 * diff34 - 144.9821 * contr12 + 34.0173 * contr13 - 49.7648 * contr14 - 48.4099 * contr23 - 445.4579 * contr24 + 64.9477 * contr34 + 2482.2293 ;
}

double DistanceEstimator::LM376()
{
	return -133.6638 * diff12 - 54.971 * diff13 - 285.8634 * diff14 + 63.2608 * diff23 + 463.5247 * diff24 + 21.55 * diff34 - 613.9563 * contr12 + 895.7548 * contr13 + 242.241 * contr23 - 5.4038 * contr24 + 124.3984 * contr34 + 2171.2721 ;
}

double DistanceEstimator::LM377()
{
	return -133.6638 * diff12 - 54.971 * diff13 - 508.6241 * diff14 + 63.2608 * diff23 + 463.5247 * diff24 + 21.55 * diff34 - 285.5896 * contr12 + 546.5198 * contr13 + 242.241 * contr23 - 5.4038 * contr24 + 124.3984 * contr34 + 2161.4613 ;
}

double DistanceEstimator::LM378()
{
	return -133.6638 * diff12 - 54.971 * diff13 + 54.8295 * diff14 + 63.2608 * diff23 + 679.3431 * diff24 + 21.55 * diff34 - 26.8581 * contr12 + 117.7271 * contr13 + 242.241 * contr23 - 5.4038 * contr24 + 124.3984 * contr34 + 2142.4757 ;
}

double DistanceEstimator::LM379()
{
	return -133.6638 * diff12 - 54.971 * diff13 + 54.8295 * diff14 + 63.2608 * diff23 - 396.2247 * diff24 + 21.55 * diff34 - 26.8581 * contr12 + 117.7271 * contr13 + 242.241 * contr23 - 5.4038 * contr24 + 124.3984 * contr34 + 2433.3253 ;
}

double DistanceEstimator::LM380()
{
	return -133.6638 * diff12 - 54.971 * diff13 + 54.8295 * diff14 + 63.2608 * diff23 - 142.4065 * diff24 + 21.55 * diff34 - 26.8581 * contr12 + 117.7271 * contr13 + 242.241 * contr23 - 5.4038 * contr24 + 124.3984 * contr34 + 2363.0048 ;
}

double DistanceEstimator::LM381()
{
	return -133.6638 * diff12 - 54.971 * diff13 + 54.8295 * diff14 + 63.2608 * diff23 + 546.5285 * diff24 + 21.55 * diff34 - 26.8581 * contr12 + 117.7271 * contr13 + 242.241 * contr23 - 5.4038 * contr24 + 124.3984 * contr34 + 2188.7681 ;
}

double DistanceEstimator::LM382()
{
	return -133.6638 * diff12 - 54.971 * diff13 + 54.8295 * diff14 + 63.2608 * diff23 + 448.4131 * diff24 + 21.55 * diff34 - 26.8581 * contr12 + 117.7271 * contr13 + 242.241 * contr23 - 5.4038 * contr24 + 124.3984 * contr34 + 2233.2078 ;
}

double DistanceEstimator::LM383()
{
	return -369.8641 * diff12 - 54.971 * diff13 - 75.3368 * diff14 + 221.3124 * diff23 + 109.9625 * diff24 + 21.55 * diff34 - 3.4741 * contr12 + 50.5445 * contr13 + 3102.9203 * contr23 - 5.4038 * contr24 + 128.3215 * contr34 + 2270.2425 ;
}

double DistanceEstimator::LM384()
{
	return -369.8641 * diff12 - 54.971 * diff13 - 75.3368 * diff14 + 221.3124 * diff23 + 109.9625 * diff24 + 21.55 * diff34 - 3.4741 * contr12 + 50.5445 * contr13 + 2269.7446 * contr23 - 5.4038 * contr24 + 128.3215 * contr34 + 2308.1268 ;
}

double DistanceEstimator::LM385()
{
	return -369.8641 * diff12 - 54.971 * diff13 - 732.4234 * diff14 + 221.3124 * diff23 + 109.9625 * diff24 + 21.55 * diff34 - 3.4741 * contr12 - 110.522 * contr13 + 1198.4552 * contr23 - 5.4038 * contr24 + 128.3215 * contr34 + 2386.6447 ;
}

double DistanceEstimator::LM386()
{
	return -461.6038 * diff12 - 54.971 * diff13 + 54.8295 * diff14 + 1107.7115 * diff23 + 109.9625 * diff24 + 21.55 * diff34 - 3.4741 * contr12 + 120.5465 * contr13 + 558.1397 * contr23 - 5.4038 * contr24 + 128.3215 * contr34 + 2337.0082 ;
}

double DistanceEstimator::LM387()
{
	return 840.2251 * diff12 + 93.0128 * diff13 + 72.2865 * diff14 - 76.4659 * diff23 + 1421.1367 * diff24 + 23.0354 * diff34 - 18.2969 * contr12 + 34.0173 * contr13 - 687.2276 * contr14 + 159.8396 * contr23 + 1.6486 * contr24 + 42.0193 * contr34 + 1779.2131 ;
}

double DistanceEstimator::LM388()
{
	return 840.2251 * diff12 + 93.0128 * diff13 + 72.2865 * diff14 - 76.4659 * diff23 + 1395.429 * diff24 + 23.0354 * diff34 - 18.2969 * contr12 + 34.0173 * contr13 - 549.4864 * contr14 + 159.8396 * contr23 + 1.6486 * contr24 + 42.0193 * contr34 + 1753.2762 ;
}

double DistanceEstimator::LM389()
{
	return 840.2251 * diff12 + 93.0128 * diff13 + 72.2865 * diff14 - 76.4659 * diff23 + 1400.2147 * diff24 + 23.0354 * diff34 - 18.2969 * contr12 + 34.0173 * contr13 - 549.4864 * contr14 + 159.8396 * contr23 + 1.6486 * contr24 + 42.0193 * contr34 + 1755.4996 ;
}

double DistanceEstimator::LM390()
{
	return 831.03 * diff12 + 93.0128 * diff13 + 72.2865 * diff14 - 76.4659 * diff23 + 1045.155 * diff24 + 23.0354 * diff34 - 18.2969 * contr12 + 34.0173 * contr13 - 322.5504 * contr14 + 159.8396 * contr23 + 1.6486 * contr24 + 42.0193 * contr34 + 1828.3934 ;
}

double DistanceEstimator::LM391()
{
	return 494.494 * diff12 + 115.7738 * diff13 + 72.2865 * diff14 - 76.4659 * diff23 + 754.762 * diff24 + 23.0354 * diff34 - 18.2969 * contr12 + 34.0173 * contr13 - 155.8525 * contr14 + 159.8396 * contr23 + 1.6486 * contr24 + 42.0193 * contr34 + 1933.1084 ;
}

double DistanceEstimator::LM392()
{
	return 125.6389 * diff12 - 134.5969 * diff13 + 72.2865 * diff14 - 76.4659 * diff23 + 232.7804 * diff24 + 23.0354 * diff34 - 18.2969 * contr12 + 34.0173 * contr13 - 29.819 * contr14 + 159.8396 * contr23 + 1.6486 * contr24 + 42.0193 * contr34 + 2186.4476 ;
}

double DistanceEstimator::LM393()
{
	return 211.4557 * diff12 - 161.9232 * diff13 + 2.9838 * diff14 + 119.5313 * diff23 + 140.9346 * diff24 + 23.0354 * diff34 - 781.8954 * contr12 + 34.0173 * contr13 + 8.3602 * contr14 + 631.3475 * contr23 - 3.6376 * contr24 + 42.0193 * contr34 + 2247.987 ;
}

double DistanceEstimator::LM394()
{
	return -30.9356 * diff12 - 161.9232 * diff13 + 2.9838 * diff14 + 119.5313 * diff23 + 140.9346 * diff24 + 23.0354 * diff34 - 46.7261 * contr12 + 34.0173 * contr13 + 8.3602 * contr14 + 631.3475 * contr23 - 3.6376 * contr24 + 42.0193 * contr34 + 2266.6312 ;
}

double DistanceEstimator::LM395()
{
	return -1.8486 * diff12 - 161.9232 * diff13 + 2.9838 * diff14 + 119.5313 * diff23 + 140.9346 * diff24 + 23.0354 * diff34 - 46.7261 * contr12 + 34.0173 * contr13 + 8.3602 * contr14 + 631.3475 * contr23 - 3.6376 * contr24 + 42.0193 * contr34 + 2255.8616 ;
}

double DistanceEstimator::LM396()
{
	return 211.4557 * diff12 - 161.9232 * diff13 + 2.9838 * diff14 + 119.5313 * diff23 + 140.9346 * diff24 + 23.0354 * diff34 - 46.7261 * contr12 + 34.0173 * contr13 + 8.3602 * contr14 + 631.3475 * contr23 - 3.6376 * contr24 + 42.0193 * contr34 + 2205.0719 ;
}

double DistanceEstimator::LM397()
{
	return 211.4557 * diff12 - 161.9232 * diff13 + 2.9838 * diff14 + 119.5313 * diff23 + 140.9346 * diff24 + 23.0354 * diff34 - 17.4494 * contr12 + 34.0173 * contr13 + 8.3602 * contr14 + 631.3475 * contr23 - 3.6376 * contr24 + 42.0193 * contr34 + 2208.9817 ;
}

double DistanceEstimator::LM398()
{
	return 211.4557 * diff12 - 161.9232 * diff13 + 2.9838 * diff14 + 175.5436 * diff23 + 140.9346 * diff24 + 23.0354 * diff34 - 57.7214 * contr12 + 34.0173 * contr13 + 8.3602 * contr14 + 1853.2362 * contr23 - 3.6376 * contr24 + 42.0193 * contr34 + 2188.0687 ;
}

double DistanceEstimator::LM399()
{
	return 211.4557 * diff12 - 161.9232 * diff13 + 2.9838 * diff14 + 175.5436 * diff23 + 140.9346 * diff24 + 23.0354 * diff34 - 57.7214 * contr12 + 34.0173 * contr13 + 8.3602 * contr14 + 739.6749 * contr23 - 3.6376 * contr24 + 42.0193 * contr34 + 2234.2144 ;
}

double DistanceEstimator::LM400()
{
	return 1363.599 * diff12 - 161.9232 * diff13 - 749.3776 * diff14 - 17.3944 * diff23 + 140.9346 * diff24 + 23.0354 * diff34 - 21.5131 * contr12 + 34.0173 * contr13 - 224.4604 * contr14 + 378.3604 * contr23 - 3.6376 * contr24 + 42.0193 * contr34 + 1957.156 ;
}

double DistanceEstimator::LM401()
{
	return 658.1404 * diff12 - 609.5144 * diff13 - 139.1221 * diff14 - 348.2573 * diff23 + 107.2459 * diff24 + 62.0827 * diff34 - 3.4741 * contr12 - 160.7073 * contr13 - 366.1726 * contr14 - 1.7082 * contr23 + 3139.5579 * contr24 - 587.4542 * contr34 + 1442.3031 ;
}

double DistanceEstimator::LM402()
{
	return 658.1404 * diff12 - 434.4178 * diff13 - 139.1221 * diff14 - 348.2573 * diff23 + 107.2459 * diff24 + 62.0827 * diff34 - 3.4741 * contr12 + 1077.0755 * contr13 - 188.172 * contr14 - 1.7082 * contr23 + 3503.4741 * contr24 - 264.3917 * contr34 + 1260.7894 ;
}

double DistanceEstimator::LM403()
{
	return 330.3849 * diff12 - 401.669 * diff13 + 116.9583 * diff14 + 248.5507 * diff23 + 181.6149 * diff24 + 62.0827 * diff34 + 32.6133 * contr12 - 151.6879 * contr13 + 33.4273 * contr23 - 172.8206 * contr24 + 53.9659 * contr34 + 2333.6464 ;
}

double DistanceEstimator::LM404()
{
	return 330.3849 * diff12 - 401.669 * diff13 + 116.9583 * diff14 + 291.268 * diff23 + 181.6149 * diff24 + 62.0827 * diff34 + 32.6133 * contr12 - 151.6879 * contr13 + 33.4273 * contr23 - 172.8206 * contr24 + 53.9659 * contr34 + 2340.9539 ;
}

double DistanceEstimator::LM405()
{
	return -1121.5602 * diff12 - 1801.0411 * diff13 + 567.343 * diff14 - 50.4703 * diff23 + 181.6149 * diff24 + 62.0827 * diff34 + 27.8721 * contr12 - 88.7551 * contr13 + 33.4273 * contr23 - 172.8206 * contr24 + 53.9659 * contr34 + 2998.2528 ;
}

double DistanceEstimator::LM406()
{
	return -1121.5602 * diff12 - 1801.0411 * diff13 + 604.3708 * diff14 - 50.4703 * diff23 + 181.6149 * diff24 + 62.0827 * diff34 + 27.8721 * contr12 - 88.7551 * contr13 + 33.4273 * contr23 - 172.8206 * contr24 + 53.9659 * contr34 + 2995.9628 ;
}

double DistanceEstimator::LM407()
{
	return -992.447 * diff12 - 1906.0781 * diff13 + 895.1907 * diff14 - 50.4703 * diff23 + 181.6149 * diff24 + 62.0827 * diff34 + 27.8721 * contr12 - 88.7551 * contr13 + 33.4273 * contr23 - 172.8206 * contr24 + 53.9659 * contr34 + 2987.3774 ;
}

double DistanceEstimator::LM408()
{
	return 221.0367 * diff12 - 843.4287 * diff13 + 305.5336 * diff14 - 50.4703 * diff23 + 181.6149 * diff24 + 62.0827 * diff34 + 27.8721 * contr12 - 118.3118 * contr13 + 33.4273 * contr23 - 172.8206 * contr24 + 53.9659 * contr34 + 2487.4999 ;
}

double DistanceEstimator::LM409()
{
	return -1112.0714 * diff12 - 843.4287 * diff13 + 305.5336 * diff14 - 50.4703 * diff23 + 181.6149 * diff24 + 62.0827 * diff34 + 27.8721 * contr12 - 118.3118 * contr13 + 33.4273 * contr23 - 172.8206 * contr24 + 53.9659 * contr34 + 2873.5257 ;
}

double DistanceEstimator::LM410()
{
	return -1425.7439 * diff12 - 843.4287 * diff13 + 305.5336 * diff14 - 50.4703 * diff23 + 181.6149 * diff24 + 62.0827 * diff34 + 27.8721 * contr12 - 118.3118 * contr13 + 33.4273 * contr23 - 172.8206 * contr24 + 53.9659 * contr34 + 2962.1632 ;
}

double DistanceEstimator::LM411()
{
	return 270.4042 * diff12 - 360.5835 * diff13 - 130.316 * diff14 - 50.4703 * diff23 + 181.6149 * diff24 + 62.0827 * diff34 + 27.8721 * contr12 - 324.6438 * contr13 + 33.4273 * contr23 - 172.8206 * contr24 + 53.9659 * contr34 + 2411.0754 ;
}

double DistanceEstimator::LM412()
{
	return -1091.8173 * diff12 - 360.5835 * diff13 - 130.316 * diff14 - 50.4703 * diff23 + 181.6149 * diff24 + 62.0827 * diff34 + 27.8721 * contr12 - 324.6438 * contr13 + 33.4273 * contr23 - 172.8206 * contr24 + 53.9659 * contr34 + 2809.9679 ;
}

double DistanceEstimator::LM413()
{
	return 330.3849 * diff12 - 468.0405 * diff13 + 24.5168 * diff14 - 50.4703 * diff23 + 181.6149 * diff24 + 62.0827 * diff34 - 456.0723 * contr12 - 126.6592 * contr13 + 576.1911 * contr23 - 611.3678 * contr24 + 53.9659 * contr34 + 2476.6285 ;
}

double DistanceEstimator::LM414()
{
	return 330.3849 * diff12 - 468.0405 * diff13 + 24.5168 * diff14 - 50.4703 * diff23 + 181.6149 * diff24 + 62.0827 * diff34 - 125.9627 * contr12 - 126.6592 * contr13 + 572.6182 * contr23 - 611.3678 * contr24 + 53.9659 * contr34 + 2431.5305 ;
}

double DistanceEstimator::LM415()
{
	return 330.3849 * diff12 - 360.7845 * diff13 + 35.2828 * diff14 - 31.8527 * diff23 + 204.2271 * diff24 + 62.0827 * diff34 - 147.4675 * contr12 + 10.6646 * contr13 + 360.8166 * contr23 - 162.0362 * contr24 + 53.9659 * contr34 + 2266.1091 ;
}

double DistanceEstimator::LM416()
{
	return 330.3849 * diff12 - 360.7845 * diff13 + 35.2828 * diff14 - 31.8527 * diff23 + 204.2271 * diff24 + 62.0827 * diff34 - 97.4535 * contr12 + 10.6646 * contr13 + 432.7902 * contr23 - 162.0362 * contr24 + 53.9659 * contr34 + 2262.1738 ;
}

double DistanceEstimator::LM417()
{
	return 330.3849 * diff12 - 410.9713 * diff13 + 35.2828 * diff14 - 31.8527 * diff23 + 204.2271 * diff24 + 62.0827 * diff34 - 59.2361 * contr12 + 10.6646 * contr13 + 145.3082 * contr23 - 278.4392 * contr24 + 53.9659 * contr34 + 2342.5718 ;
}

double DistanceEstimator::LM418()
{
	return -210.8785 * diff12 - 410.9713 * diff13 + 35.2828 * diff14 - 31.8527 * diff23 + 204.2271 * diff24 + 62.0827 * diff34 - 83.1348 * contr12 + 10.6646 * contr13 + 145.3082 * contr23 - 410.7823 * contr24 + 53.9659 * contr34 + 2525.5593 ;
}

double DistanceEstimator::LM419()
{
	return -106.3145 * diff12 - 410.9713 * diff13 + 35.2828 * diff14 - 31.8527 * diff23 + 204.2271 * diff24 + 62.0827 * diff34 - 83.1348 * contr12 + 10.6646 * contr13 + 145.3082 * contr23 - 410.7823 * contr24 + 53.9659 * contr34 + 2488.9499 ;
}

double DistanceEstimator::LM420()
{
	return 330.3849 * diff12 - 577.6013 * diff13 + 35.2828 * diff14 - 31.8527 * diff23 + 204.2271 * diff24 + 62.0827 * diff34 - 63.0173 * contr12 + 10.6646 * contr13 + 145.3082 * contr23 - 261.2882 * contr24 + 53.9659 * contr34 + 2360.9425 ;
}

double DistanceEstimator::LM421()
{
	return -940.834 * diff12 - 345.5871 * diff13 - 326.4717 * diff14 + 116.6979 * diff23 + 15.1806 * diff24 + 26.8386 * diff34 + 131.9588 * contr12 + 13.2355 * contr13 + 25.7371 * contr14 + 7.3389 * contr23 - 287.535 * contr24 - 4.1661 * contr34 + 2772.442 ;
}

double DistanceEstimator::LM422()
{
	return -940.834 * diff12 - 345.5871 * diff13 - 326.4717 * diff14 + 252.4788 * diff23 + 15.1806 * diff24 + 26.8386 * diff34 + 131.9588 * contr12 + 13.2355 * contr13 + 25.7371 * contr14 + 7.3389 * contr23 - 287.535 * contr24 - 4.1661 * contr34 + 2762.5687 ;
}

double DistanceEstimator::LM423()
{
	return -1763.432 * diff12 - 262.0516 * diff13 + 707.6503 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 26.8386 * diff34 + 1178.2001 * contr12 - 417.4453 * contr13 + 25.7371 * contr14 - 854.2647 * contr23 - 215.9527 * contr24 - 4.1661 * contr34 + 2847.9748 ;
}

double DistanceEstimator::LM424()
{
	return -1763.432 * diff12 - 262.0516 * diff13 + 707.6503 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 26.8386 * diff34 + 1137.7121 * contr12 - 417.4453 * contr13 + 25.7371 * contr14 - 854.2647 * contr23 - 215.9527 * contr24 - 4.1661 * contr34 + 2862.7088 ;
}

double DistanceEstimator::LM425()
{
	return -2745.4754 * diff12 - 262.0516 * diff13 + 184.6164 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 26.8386 * diff34 + 550.703 * contr12 + 976.2684 * contr13 + 25.7371 * contr14 - 108.4842 * contr23 + 1338.8849 * contr24 - 4.1661 * contr34 + 2584.144 ;
}

double DistanceEstimator::LM426()
{
	return -2745.4754 * diff12 - 262.0516 * diff13 + 184.6164 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 26.8386 * diff34 + 550.703 * contr12 + 759.6078 * contr13 + 25.7371 * contr14 - 108.4842 * contr23 + 1338.8849 * contr24 - 4.1661 * contr34 + 2630.1016 ;
}

double DistanceEstimator::LM427()
{
	return -2745.4754 * diff12 - 262.0516 * diff13 + 184.6164 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 26.8386 * diff34 + 550.703 * contr12 + 794.4905 * contr13 + 25.7371 * contr14 - 108.4842 * contr23 + 1338.8849 * contr24 - 4.1661 * contr34 + 2622.6539 ;
}

double DistanceEstimator::LM428()
{
	return -3765.1431 * diff12 - 262.0516 * diff13 + 184.6164 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 26.8386 * diff34 - 58.7896 * contr12 + 238.3121 * contr13 + 25.7371 * contr14 - 254.4424 * contr23 - 476.4734 * contr24 - 4.1661 * contr34 + 3690.7674 ;
}

double DistanceEstimator::LM429()
{
	return -4123.2843 * diff12 - 262.0516 * diff13 + 184.6164 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 26.8386 * diff34 - 323.3485 * contr12 + 238.3121 * contr13 + 25.7371 * contr14 - 254.4424 * contr23 - 637.2356 * contr24 - 4.1661 * contr34 + 3901.5857 ;
}

double DistanceEstimator::LM430()
{
	return -2952.9493 * diff12 - 262.0516 * diff13 + 184.6164 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 26.8386 * diff34 + 280.3748 * contr12 + 238.3121 * contr13 + 25.7371 * contr14 - 254.4424 * contr23 + 73.1266 * contr24 - 4.1661 * contr34 + 3228.1732 ;
}

double DistanceEstimator::LM431()
{
	return -1222.6348 * diff12 - 262.0516 * diff13 - 223.6052 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 26.8386 * diff34 + 163.2267 * contr12 + 67.6492 * contr13 + 25.7371 * contr14 + 7.3389 * contr23 - 215.9527 * contr24 - 4.1661 * contr34 + 2808.7435 ;
}

double DistanceEstimator::LM432()
{
	return -530.2133 * diff12 - 518.2544 * diff13 - 137.3024 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 360.0207 * diff34 + 214.641 * contr12 + 107.9309 * contr13 + 42.0493 * contr14 + 7.3389 * contr23 - 180.0055 * contr24 - 4.1661 * contr34 + 2651.595 ;
}

double DistanceEstimator::LM433()
{
	return -256.957 * diff12 - 1247.8439 * diff13 - 137.3024 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 620.5495 * diff34 + 165.7758 * contr12 + 102.336 * contr13 + 42.0493 * contr14 + 7.3389 * contr23 - 180.0055 * contr24 - 4.1661 * contr34 + 2580.4157 ;
}

double DistanceEstimator::LM434()
{
	return -396.5553 * diff12 - 862.8204 * diff13 - 137.3024 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 1042.0575 * diff34 - 342.8608 * contr12 + 237.385 * contr13 + 42.0493 * contr14 + 7.3389 * contr23 - 180.0055 * contr24 - 4.1661 * contr34 + 2639.7208 ;
}

double DistanceEstimator::LM435()
{
	return -396.5553 * diff12 - 862.8204 * diff13 - 137.3024 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 1042.0575 * diff34 - 342.8608 * contr12 + 237.385 * contr13 + 42.0493 * contr14 + 7.3389 * contr23 - 180.0055 * contr24 - 4.1661 * contr34 + 2643.501 ;
}

double DistanceEstimator::LM436()
{
	return -1802.0485 * diff12 - 862.8204 * diff13 - 137.3024 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 1725.3091 * diff34 - 243.9593 * contr12 + 213.1743 * contr13 + 42.0493 * contr14 + 7.3389 * contr23 - 180.0055 * contr24 - 754.975 * contr34 + 3104.0376 ;
}

double DistanceEstimator::LM437()
{
	return -568.7211 * diff12 - 152.7287 * diff13 - 40.3343 * diff14 - 166.7921 * diff23 + 15.1806 * diff24 + 177.9833 * diff34 - 93.8101 * contr12 + 13.2355 * contr13 + 893.3525 * contr14 + 6.2253 * contr23 - 51.7166 * contr24 - 4.1661 * contr34 + 2774.6953 ;
}

double DistanceEstimator::LM438()
{
	return -568.7211 * diff12 - 152.7287 * diff13 - 40.3343 * diff14 - 118.3679 * diff23 + 15.1806 * diff24 + 177.9833 * diff34 - 93.8101 * contr12 + 13.2355 * contr13 - 167.7306 * contr14 + 6.2253 * contr23 - 51.7166 * contr24 - 4.1661 * contr34 + 2796.0495 ;
}

double DistanceEstimator::LM439()
{
	return -1235.3438 * diff12 - 175.8238 * diff13 - 40.3343 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 110.1923 * diff34 - 109.0239 * contr12 + 39.9087 * contr13 - 206.8607 * contr14 + 6.2253 * contr23 - 51.7166 * contr24 - 4.1661 * contr34 + 2943.7712 ;
}

double DistanceEstimator::LM440()
{
	return -1194.5981 * diff12 - 293.6478 * diff13 - 40.3343 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 110.1923 * diff34 - 109.0239 * contr12 + 39.9087 * contr13 - 206.8607 * contr14 + 6.2253 * contr23 - 51.7166 * contr24 - 4.1661 * contr34 + 2926.8414 ;
}

double DistanceEstimator::LM441()
{
	return -1194.5981 * diff12 - 199.0137 * diff13 - 40.3343 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 110.1923 * diff34 - 109.0239 * contr12 + 39.9087 * contr13 - 206.8607 * contr14 + 6.2253 * contr23 - 51.7166 * contr24 - 4.1661 * contr34 + 2907.5528 ;
}

double DistanceEstimator::LM442()
{
	return -1194.5981 * diff12 - 148.4146 * diff13 - 40.3343 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 110.1923 * diff34 - 109.0239 * contr12 + 39.9087 * contr13 - 206.8607 * contr14 + 6.2253 * contr23 - 51.7166 * contr24 - 4.1661 * contr34 + 2910.1625 ;
}

double DistanceEstimator::LM443()
{
	return -1332.0068 * diff12 - 107.4421 * diff13 - 40.3343 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 + 766.1825 * diff34 - 1028.3139 * contr12 + 310.9896 * contr13 + 582.9815 * contr14 + 6.2253 * contr23 - 51.7166 * contr24 - 4.1661 * contr34 + 2986.9234 ;
}

double DistanceEstimator::LM444()
{
	return -1357.6282 * diff12 + 418.4908 * diff13 - 40.3343 * diff14 - 330.7102 * diff23 + 15.1806 * diff24 + 110.1923 * diff34 + 38.3143 * contr12 + 133.8555 * contr13 + 274.7046 * contr14 + 6.2253 * contr23 - 51.7166 * contr24 - 4.1661 * contr34 + 2919.8123 ;
}

double DistanceEstimator::LM445()
{
	return -1357.6282 * diff12 + 476.9278 * diff13 - 40.3343 * diff14 - 330.7102 * diff23 + 15.1806 * diff24 + 110.1923 * diff34 + 38.3143 * contr12 + 133.8555 * contr13 + 274.7046 * contr14 + 6.2253 * contr23 - 51.7166 * contr24 - 4.1661 * contr34 + 2920.0212 ;
}

double DistanceEstimator::LM446()
{
	return -1357.6282 * diff12 - 107.4421 * diff13 - 40.3343 * diff14 - 193.5403 * diff23 + 15.1806 * diff24 + 110.1923 * diff34 + 38.3143 * contr12 + 133.8555 * contr13 + 274.7046 * contr14 + 6.2253 * contr23 - 51.7166 * contr24 - 4.1661 * contr34 + 2911.5635 ;
}

double DistanceEstimator::LM447()
{
	return -1294.6484 * diff12 - 107.4421 * diff13 - 40.3343 * diff14 - 50.4894 * diff23 + 15.1806 * diff24 + 110.1923 * diff34 + 38.3143 * contr12 + 133.8555 * contr13 + 729.8377 * contr14 + 6.2253 * contr23 - 51.7166 * contr24 - 4.1661 * contr34 + 2819.3194 ;
}

double DistanceEstimator::LM448()
{
	return -372.6074 * diff12 - 42.047 * diff13 + 171.3126 * diff14 + 332.0356 * diff23 + 15.1806 * diff24 - 18.7195 * diff34 + 119.3825 * contr12 + 213.1259 * contr13 + 521.5518 * contr23 + 809.9586 * contr24 + 371.1865 * contr34 + 2111.0572 ;
}

double DistanceEstimator::LM449()
{
	return -607.617 * diff12 - 42.047 * diff13 + 171.3126 * diff14 + 376.7186 * diff23 + 15.1806 * diff24 - 18.7195 * diff34 + 240.6125 * contr12 + 213.1259 * contr13 + 521.5518 * contr23 - 151.3161 * contr24 + 371.1865 * contr34 + 2427.4278 ;
}

double DistanceEstimator::LM450()
{
	return -607.617 * diff12 - 42.047 * diff13 + 171.3126 * diff14 + 376.7186 * diff23 + 15.1806 * diff24 - 18.7195 * diff34 + 81.9935 * contr12 + 213.1259 * contr13 + 521.5518 * contr23 - 151.3161 * contr24 + 371.1865 * contr34 + 2460.5431 ;
}

double DistanceEstimator::LM451()
{
	return -607.617 * diff12 - 42.047 * diff13 + 171.3126 * diff14 + 376.7186 * diff23 + 15.1806 * diff24 - 18.7195 * diff34 + 29.7151 * contr12 + 213.1259 * contr13 + 521.5518 * contr23 - 151.3161 * contr24 + 371.1865 * contr34 + 2464.1216 ;
}

double DistanceEstimator::LM452()
{
	return -1681.0922 * diff12 - 42.047 * diff13 + 171.3126 * diff14 + 376.7186 * diff23 + 15.1806 * diff24 - 18.7195 * diff34 + 119.3825 * contr12 + 213.1259 * contr13 + 521.5518 * contr23 - 781.8027 * contr24 + 371.1865 * contr34 + 2882.9397 ;
}

double DistanceEstimator::LM453()
{
	return -524.7971 * diff12 - 42.047 * diff13 + 171.3126 * diff14 + 426.6564 * diff23 + 15.1806 * diff24 - 18.7195 * diff34 - 1171.249 * contr12 + 1522.6177 * contr13 - 2731.4743 * contr23 + 3169.6473 * contr24 + 371.1865 * contr34 + 1669.0134 ;
}

double DistanceEstimator::LM454()
{
	return -524.7971 * diff12 - 42.047 * diff13 + 171.3126 * diff14 + 426.6564 * diff23 + 15.1806 * diff24 - 18.7195 * diff34 - 1171.249 * contr12 + 1488.4311 * contr13 - 2731.4743 * contr23 + 3169.6473 * contr24 + 371.1865 * contr34 + 1684.7979 ;
}

double DistanceEstimator::LM455()
{
	return -3097.666 * diff12 - 42.047 * diff13 + 171.3126 * diff14 + 426.6564 * diff23 + 15.1806 * diff24 - 18.7195 * diff34 - 2109.8901 * contr12 + 599.5805 * contr13 - 5503.1183 * contr23 + 3169.6473 * contr24 + 371.1865 * contr34 + 2722.0806 ;
}

double DistanceEstimator::LM456()
{
	return -2962.2518 * diff12 - 42.047 * diff13 + 171.3126 * diff14 + 426.6564 * diff23 + 15.1806 * diff24 - 18.7195 * diff34 - 2025.7615 * contr12 + 599.5805 * contr13 - 5503.1183 * contr23 + 3169.6473 * contr24 + 371.1865 * contr34 + 2641.9096 ;
}

double DistanceEstimator::LM457()
{
	return -144.3405 * diff12 - 36.7003 * diff13 + 95.878 * diff14 + 15.0058 * diff23 + 15.1806 * diff24 - 18.7195 * diff34 - 1.9821 * contr12 + 13.2355 * contr13 + 99.0705 * contr23 - 59.9614 * contr24 + 101.9407 * contr34 + 2365.7636 ;
}

double DistanceEstimator::LM458()
{
	return -720.0922 * diff12 - 36.7003 * diff13 + 96.8258 * diff14 + 53.9456 * diff23 + 15.1806 * diff24 - 18.7195 * diff34 - 378.0691 * contr12 + 13.2355 * contr13 + 99.0705 * contr23 - 59.9614 * contr24 + 101.9407 * contr34 + 2616.027 ;
}

double DistanceEstimator::LM459()
{
	return -720.0922 * diff12 - 36.7003 * diff13 + 96.8258 * diff14 + 53.9456 * diff23 + 15.1806 * diff24 - 18.7195 * diff34 - 405.2959 * contr12 + 13.2355 * contr13 + 99.0705 * contr23 - 59.9614 * contr24 + 101.9407 * contr34 + 2601.7994 ;
}

double DistanceEstimator::LM460()
{
	return -512.2179 * diff12 - 36.7003 * diff13 + 96.8258 * diff14 + 53.9456 * diff23 + 15.1806 * diff24 - 18.7195 * diff34 - 59.2242 * contr12 + 13.2355 * contr13 + 99.0705 * contr23 - 59.9614 * contr24 + 101.9407 * contr34 + 2487.5205 ;
}

double DistanceEstimator::LM461()
{
	return -290.7067 * diff12 - 36.7003 * diff13 + 96.8258 * diff14 + 51.7218 * diff23 + 15.1806 * diff24 - 18.7195 * diff34 - 2.3351 * contr12 + 13.2355 * contr13 + 99.0705 * contr23 - 59.9614 * contr24 + 101.9407 * contr34 + 2437.0979 ;
}

double DistanceEstimator::LM462()
{
	return -132.1614 * diff12 - 981.5373 * diff13 + 85.6672 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 - 18.7195 * diff34 - 1277.9733 * contr12 + 13.2355 * contr13 + 144.8798 * contr23 - 1912.2655 * contr24 + 101.9407 * contr34 + 3155.8692 ;
}

double DistanceEstimator::LM463()
{
	return -1487.1001 * diff12 - 7.4506 * diff13 + 820.9277 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 - 91.7259 * diff34 - 125.0789 * contr12 + 13.2355 * contr13 + 47.5774 * contr23 - 1422.3325 * contr24 + 172.4675 * contr34 + 3166.0012 ;
}

double DistanceEstimator::LM464()
{
	return -167.335 * diff12 - 240.0272 * diff13 + 1218.0128 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 - 91.7259 * diff34 + 279.2567 * contr12 + 13.2355 * contr13 + 47.5774 * contr23 - 1368.522 * contr24 + 172.4675 * contr34 + 2710.291 ;
}

double DistanceEstimator::LM465()
{
	return -167.335 * diff12 - 7.4506 * diff13 + 1205.0183 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 - 91.7259 * diff34 - 301.4145 * contr12 + 13.2355 * contr13 + 47.5774 * contr23 - 1368.522 * contr24 + 172.4675 * contr34 + 2806.3863 ;
}

double DistanceEstimator::LM466()
{
	return -167.335 * diff12 - 7.4506 * diff13 + 1205.0183 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 - 91.7259 * diff34 - 301.4145 * contr12 + 13.2355 * contr13 + 47.5774 * contr23 - 1368.522 * contr24 + 172.4675 * contr34 + 2805.9176 ;
}

double DistanceEstimator::LM467()
{
	return -295.1137 * diff12 - 7.4506 * diff13 + 1205.0183 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 - 91.7259 * diff34 - 301.4145 * contr12 + 13.2355 * contr13 + 47.5774 * contr23 - 1368.522 * contr24 + 172.4675 * contr34 + 2836.3322 ;
}

double DistanceEstimator::LM468()
{
	return -276.8596 * diff12 - 7.4506 * diff13 + 1205.0183 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 - 91.7259 * diff34 - 301.4145 * contr12 + 13.2355 * contr13 + 47.5774 * contr23 - 1368.522 * contr24 + 172.4675 * contr34 + 2830.7066 ;
}

double DistanceEstimator::LM469()
{
	return -167.335 * diff12 - 75.1004 * diff13 + 118.9582 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 - 81.1648 * diff34 - 29.0789 * contr12 + 13.2355 * contr13 + 47.5774 * contr23 - 255.075 * contr24 + 75.4633 * contr34 + 2532.2826 ;
}

double DistanceEstimator::LM470()
{
	return -167.335 * diff12 - 40.2185 * diff13 + 118.9582 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 - 81.1648 * diff34 - 29.0789 * contr12 + 13.2355 * contr13 + 47.5774 * contr23 - 255.075 * contr24 + 75.4633 * contr34 + 2508.8931 ;
}

double DistanceEstimator::LM471()
{
	return -230.1464 * diff12 - 7.4506 * diff13 + 43.4583 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 - 98.8427 * diff34 - 29.0789 * contr12 + 13.2355 * contr13 + 47.5774 * contr23 - 191.299 * contr24 + 54.2994 * contr34 + 2567.5427 ;
}

double DistanceEstimator::LM472()
{
	return -257.3256 * diff12 - 48.0541 * diff13 + 181.5661 * diff14 + 37.8091 * diff23 + 15.1806 * diff24 - 113.4807 * diff34 - 109.7416 * contr12 + 13.2355 * contr13 + 103.9303 * contr23 - 64.4467 * contr24 + 39.9383 * contr34 + 2504.7299 ;
}

double DistanceEstimator::LM473()
{
	return -257.3256 * diff12 - 54.974 * diff13 + 181.5661 * diff14 + 37.8091 * diff23 + 15.1806 * diff24 - 113.4807 * diff34 - 109.7416 * contr12 + 13.2355 * contr13 + 103.9303 * contr23 - 64.4467 * contr24 + 195.333 * contr34 + 2449.3426 ;
}

double DistanceEstimator::LM474()
{
	return -257.3256 * diff12 - 54.974 * diff13 + 181.5661 * diff14 + 37.8091 * diff23 + 15.1806 * diff24 - 113.4807 * diff34 - 109.7416 * contr12 + 13.2355 * contr13 + 103.9303 * contr23 - 64.4467 * contr24 + 385.1098 * contr34 + 2420.0435 ;
}

double DistanceEstimator::LM475()
{
	return -257.3256 * diff12 + 15.5978 * diff13 + 210.1401 * diff14 + 223.1402 * diff23 + 15.1806 * diff24 - 113.4807 * diff34 - 758.4989 * contr12 + 13.2355 * contr13 + 103.9303 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2583.3751 ;
}

double DistanceEstimator::LM476()
{
	return -257.3256 * diff12 + 15.5978 * diff13 + 210.1401 * diff14 + 223.1402 * diff23 + 15.1806 * diff24 - 113.4807 * diff34 - 632.3738 * contr12 + 13.2355 * contr13 + 103.9303 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2554.5663 ;
}

double DistanceEstimator::LM477()
{
	return -257.3256 * diff12 + 15.5978 * diff13 + 210.1401 * diff14 + 131.0609 * diff23 + 15.1806 * diff24 - 113.4807 * diff34 - 201.9307 * contr12 + 13.2355 * contr13 + 103.9303 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2529.4013 ;
}

double DistanceEstimator::LM478()
{
	return -427.3016 * diff12 - 130.9729 * diff13 + 1941.6245 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 - 898.1658 * diff34 + 977.2943 * contr12 + 13.2355 * contr13 - 434.7129 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2410.7032 ;
}

double DistanceEstimator::LM479()
{
	return -1682.422 * diff12 - 130.9729 * diff13 + 850.729 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 - 2378.9966 * diff34 + 2666.6403 * contr12 + 13.2355 * contr13 - 418.0671 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2680.6896 ;
}

double DistanceEstimator::LM480()
{
	return -2179.7795 * diff12 - 130.9729 * diff13 + 850.729 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 - 2378.9966 * diff34 + 3799.0321 * contr12 + 13.2355 * contr13 - 418.0671 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2566.6277 ;
}

double DistanceEstimator::LM481()
{
	return -2350.6139 * diff12 - 130.9729 * diff13 + 850.729 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 - 2378.9966 * diff34 + 3799.0321 * contr12 + 13.2355 * contr13 - 418.0671 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2615.2179 ;
}

double DistanceEstimator::LM482()
{
	return -427.3016 * diff12 - 130.9729 * diff13 + 850.729 * diff14 - 15.2034 * diff23 + 15.1806 * diff24 - 709.8414 * diff34 + 725.7915 * contr12 + 13.2355 * contr13 - 438.9527 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2569.5804 ;
}

double DistanceEstimator::LM483()
{
	return -695.9647 * diff12 + 28.0386 * diff13 + 43.4583 * diff14 + 118.3355 * diff23 + 15.1806 * diff24 - 158.4841 * diff34 - 97.3523 * contr12 + 13.2355 * contr13 + 251.2582 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2690.1952 ;
}

double DistanceEstimator::LM484()
{
	return -695.9647 * diff12 + 28.0386 * diff13 + 43.4583 * diff14 + 118.3355 * diff23 + 15.1806 * diff24 - 158.4841 * diff34 + 486.2909 * contr12 + 13.2355 * contr13 + 251.2582 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2628.3982 ;
}

double DistanceEstimator::LM485()
{
	return -695.9647 * diff12 + 28.0386 * diff13 + 43.4583 * diff14 + 118.3355 * diff23 + 15.1806 * diff24 - 158.4841 * diff34 + 515.473 * contr12 + 13.2355 * contr13 + 251.2582 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2628.6087 ;
}

double DistanceEstimator::LM486()
{
	return -454.801 * diff12 + 28.0386 * diff13 + 43.4583 * diff14 + 118.3355 * diff23 + 15.1806 * diff24 - 158.4841 * diff34 - 97.3523 * contr12 + 13.2355 * contr13 + 1808.8598 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2561.8944 ;
}

double DistanceEstimator::LM487()
{
	return -454.801 * diff12 + 28.0386 * diff13 + 43.4583 * diff14 + 118.3355 * diff23 + 15.1806 * diff24 - 158.4841 * diff34 - 97.3523 * contr12 + 13.2355 * contr13 + 1908.6862 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2563.149 ;
}

double DistanceEstimator::LM488()
{
	return -526.5174 * diff12 + 28.0386 * diff13 + 43.4583 * diff14 + 118.3355 * diff23 + 15.1806 * diff24 - 158.4841 * diff34 - 97.3523 * contr12 + 13.2355 * contr13 + 630.0612 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2618.6999 ;
}

double DistanceEstimator::LM489()
{
	return -742.0707 * diff12 - 30.356 * diff13 + 43.4583 * diff14 + 118.3355 * diff23 + 15.1806 * diff24 - 158.4841 * diff34 - 97.3523 * contr12 + 13.2355 * contr13 + 251.2582 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2687.1455 ;
}

double DistanceEstimator::LM490()
{
	return -562.3449 * diff12 + 111.9329 * diff13 + 43.4583 * diff14 + 1176.2542 * diff23 + 15.1806 * diff24 - 158.4841 * diff34 + 67.38 * contr12 + 13.2355 * contr13 + 425.6791 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2428.8213 ;
}

double DistanceEstimator::LM491()
{
	return -562.3449 * diff12 + 111.9329 * diff13 + 43.4583 * diff14 + 1176.2542 * diff23 + 15.1806 * diff24 - 158.4841 * diff34 + 67.38 * contr12 + 13.2355 * contr13 + 425.6791 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2429.6845 ;
}

double DistanceEstimator::LM492()
{
	return -562.3449 * diff12 + 111.9329 * diff13 + 43.4583 * diff14 + 1176.2542 * diff23 + 15.1806 * diff24 - 158.4841 * diff34 - 62.0546 * contr12 + 13.2355 * contr13 + 425.6791 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2440.7513 ;
}

double DistanceEstimator::LM493()
{
	return -562.3449 * diff12 + 111.9329 * diff13 + 43.4583 * diff14 + 1222.5341 * diff23 + 15.1806 * diff24 - 158.4841 * diff34 - 62.0546 * contr12 + 13.2355 * contr13 + 425.6791 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2438.7486 ;
}

double DistanceEstimator::LM494()
{
	return -562.3449 * diff12 + 111.9329 * diff13 + 43.4583 * diff14 + 1027.5194 * diff23 + 15.1806 * diff24 - 158.4841 * diff34 - 36.842 * contr12 + 13.2355 * contr13 + 425.6791 * contr23 - 64.4467 * contr24 + 1.8998 * contr34 + 2499.0114 ;
}

double DistanceEstimator::LM495()
{
	return 108.3105 * diff12 - 260.9767 * diff13 - 14.8308 * diff14 + 22.8605 * diff23 - 17.2651 * diff24 - 15.9321 * diff34 + 44.9023 * contr12 - 24.2735 * contr13 + 35.0513 * contr14 + 182.3846 * contr23 + 23.7575 * contr24 + 16.1697 * contr34 + 2265.2489 ;
}

double DistanceEstimator::LM496()
{
	return 591.2459 * diff12 - 260.9767 * diff13 - 14.8308 * diff14 + 22.8605 * diff23 - 17.2651 * diff24 - 15.9321 * diff34 - 115.3388 * contr12 - 24.2735 * contr13 + 51.1187 * contr14 + 198.3816 * contr23 + 23.7575 * contr24 + 16.1697 * contr34 + 2193.7692 ;
}

double DistanceEstimator::LM497()
{
	return 600.8171 * diff12 - 260.9767 * diff13 - 14.8308 * diff14 + 22.8605 * diff23 - 17.2651 * diff24 - 15.9321 * diff34 - 40.3153 * contr12 - 24.2735 * contr13 + 51.1187 * contr14 + 198.3816 * contr23 + 23.7575 * contr24 + 16.1697 * contr34 + 2169.9216 ;
}

double DistanceEstimator::LM498()
{
	return 789.1995 * diff12 - 260.9767 * diff13 - 14.8308 * diff14 + 22.8605 * diff23 - 17.2651 * diff24 - 15.9321 * diff34 - 40.3153 * contr12 - 24.2735 * contr13 + 51.1187 * contr14 + 198.3816 * contr23 + 23.7575 * contr24 + 16.1697 * contr34 + 2128.339 ;
}

double DistanceEstimator::LM499()
{
	return 282.7907 * diff12 - 260.9767 * diff13 - 14.8308 * diff14 + 22.8605 * diff23 - 17.2651 * diff24 - 15.9321 * diff34 + 18.0988 * contr12 - 24.2735 * contr13 - 755.5159 * contr14 + 198.3816 * contr23 + 23.7575 * contr24 + 16.1697 * contr34 + 2288.6409 ;
}

double DistanceEstimator::LM500()
{
	return 282.7907 * diff12 - 260.9767 * diff13 - 14.8308 * diff14 + 22.8605 * diff23 - 17.2651 * diff24 - 15.9321 * diff34 + 18.0988 * contr12 - 24.2735 * contr13 - 687.6599 * contr14 + 198.3816 * contr23 + 23.7575 * contr24 + 16.1697 * contr34 + 2282.1316 ;
}

double DistanceEstimator::LM501()
{
	return 282.7907 * diff12 - 260.9767 * diff13 - 14.8308 * diff14 + 22.8605 * diff23 - 17.2651 * diff24 - 15.9321 * diff34 + 18.0988 * contr12 - 24.2735 * contr13 - 114.7026 * contr14 + 198.3816 * contr23 + 23.7575 * contr24 + 16.1697 * contr34 + 2249.4148 ;
}

double DistanceEstimator::LM502()
{
	return 202.2443 * diff12 - 260.9767 * diff13 - 14.8308 * diff14 + 22.8605 * diff23 - 17.2651 * diff24 - 15.9321 * diff34 + 45.9719 * contr12 - 24.2735 * contr13 + 65.7978 * contr14 + 198.3816 * contr23 + 23.7575 * contr24 + 16.1697 * contr34 + 2259.2062 ;
}

double DistanceEstimator::LM503()
{
	return 100.2988 * diff12 - 65.9062 * diff13 - 14.8308 * diff14 + 17.9235 * diff23 + 91.3079 * diff24 - 15.9321 * diff34 + 35.32 * contr12 - 21.7697 * contr13 + 38.0318 * contr14 + 127.4331 * contr23 + 14.7566 * contr24 + 16.1697 * contr34 + 2176.9839 ;
}

double DistanceEstimator::LM504()
{
	return 197.8074 * diff12 - 501.0604 * diff13 - 14.8308 * diff14 + 17.9235 * diff23 + 161.9025 * diff24 - 15.9321 * diff34 + 15.0742 * contr12 - 21.7697 * contr13 + 52.6139 * contr14 + 124.3859 * contr23 - 12.5435 * contr24 + 16.1697 * contr34 + 2294.8712 ;
}

double DistanceEstimator::LM505()
{
	return 529.3093 * diff12 - 4823.4789 * diff13 - 14.8308 * diff14 + 17.9235 * diff23 + 161.9025 * diff24 - 15.9321 * diff34 + 15.0742 * contr12 - 21.7697 * contr13 + 52.6139 * contr14 + 124.3859 * contr23 - 12.5435 * contr24 + 16.1697 * contr34 + 3447.4327 ;
}

double DistanceEstimator::LM506()
{
	return 529.3093 * diff12 - 4131.6111 * diff13 - 14.8308 * diff14 + 17.9235 * diff23 + 161.9025 * diff24 - 15.9321 * diff34 + 15.0742 * contr12 - 21.7697 * contr13 + 52.6139 * contr14 + 124.3859 * contr23 - 12.5435 * contr24 + 16.1697 * contr34 + 3245.2165 ;
}

double DistanceEstimator::LM507()
{
	return 246.6503 * diff12 - 1134.2275 * diff13 - 14.8308 * diff14 + 17.9235 * diff23 + 161.9025 * diff24 - 15.9321 * diff34 + 15.0742 * contr12 - 21.7697 * contr13 + 52.6139 * contr14 + 124.3859 * contr23 - 12.5435 * contr24 + 16.1697 * contr34 + 2447.2314 ;
}

double DistanceEstimator::LM508()
{
	return 254.3838 * diff12 - 1134.2275 * diff13 - 14.8308 * diff14 + 17.9235 * diff23 + 161.9025 * diff24 - 15.9321 * diff34 + 15.0742 * contr12 - 21.7697 * contr13 + 52.6139 * contr14 + 124.3859 * contr23 - 12.5435 * contr24 + 16.1697 * contr34 + 2443.679 ;
}

double DistanceEstimator::LM509()
{
	return 401.3195 * diff12 - 1134.2275 * diff13 - 14.8308 * diff14 + 17.9235 * diff23 + 161.9025 * diff24 - 15.9321 * diff34 + 15.0742 * contr12 - 21.7697 * contr13 + 52.6139 * contr14 + 124.3859 * contr23 - 12.5435 * contr24 + 16.1697 * contr34 + 2404.3761 ;
}

double DistanceEstimator::LM510()
{
	return 317.3497 * diff12 - 766.9952 * diff13 - 14.8308 * diff14 + 17.9235 * diff23 + 161.9025 * diff24 - 15.9321 * diff34 + 15.0742 * contr12 - 21.7697 * contr13 + 52.6139 * contr14 + 124.3859 * contr23 - 12.5435 * contr24 + 16.1697 * contr34 + 2330.4491 ;
}

double DistanceEstimator::LM511()
{
	return 159.2701 * diff12 - 76.4766 * diff13 - 14.8308 * diff14 + 17.9235 * diff23 + 896.4619 * diff24 - 15.9321 * diff34 - 327.1344 * contr12 - 21.7697 * contr13 + 76.0672 * contr14 + 124.3859 * contr23 - 54.6736 * contr24 + 16.1697 * contr34 + 2116.31 ;
}

double DistanceEstimator::LM512()
{
	return 21.7909 * diff12 - 2142.0356 * diff13 - 14.8134 * diff14 - 36.2594 * diff23 - 52.4775 * diff24 - 15.9321 * diff34 - 375.251 * contr12 - 11.2403 * contr13 + 25.5789 * contr14 + 21.2244 * contr23 + 23.7371 * contr24 + 16.1697 * contr34 + 2863.4671 ;
}

double DistanceEstimator::LM513()
{
	return 21.7909 * diff12 - 260.9767 * diff13 - 14.8134 * diff14 - 1175.0426 * diff23 - 52.4775 * diff24 - 15.9321 * diff34 - 1.0073 * contr12 - 11.2403 * contr13 + 33.2218 * contr14 + 21.2244 * contr23 + 23.7371 * contr24 + 16.1697 * contr34 + 2325.9303 ;
}

double DistanceEstimator::LM514()
{
	return 162.5243 * diff12 - 1610.4216 * diff13 + 372.7231 * diff14 - 173.6517 * diff23 - 52.4775 * diff24 - 15.9321 * diff34 - 155.4861 * contr12 - 11.2403 * contr13 + 281.6042 * contr14 + 21.2244 * contr23 + 23.7371 * contr24 + 16.1697 * contr34 + 2576.9793 ;
}

double DistanceEstimator::LM515()
{
	return 162.5243 * diff12 - 1862.2613 * diff13 + 852.1 * diff14 - 173.6517 * diff23 - 52.4775 * diff24 - 15.9321 * diff34 - 807.7254 * contr12 - 11.2403 * contr13 + 311.6377 * contr14 + 21.2244 * contr23 + 23.7371 * contr24 + 16.1697 * contr34 + 2690.1585 ;
}

double DistanceEstimator::LM516()
{
	return 162.5243 * diff12 - 1862.2613 * diff13 + 1019.6203 * diff14 - 173.6517 * diff23 - 52.4775 * diff24 - 15.9321 * diff34 - 224.2888 * contr12 - 11.2403 * contr13 + 311.6377 * contr14 + 21.2244 * contr23 + 23.7371 * contr24 + 16.1697 * contr34 + 2583.74 ;
}

double DistanceEstimator::LM517()
{
	return 162.5243 * diff12 - 1404.6743 * diff13 + 390.7923 * diff14 - 173.6517 * diff23 - 52.4775 * diff24 - 15.9321 * diff34 - 160.5364 * contr12 - 11.2403 * contr13 + 252.8266 * contr14 + 21.2244 * contr23 + 23.7371 * contr24 + 16.1697 * contr34 + 2506.0701 ;
}

double DistanceEstimator::LM518()
{
	return 370.9181 * diff12 - 1654.728 * diff13 - 14.8134 * diff14 - 173.6517 * diff23 - 52.4775 * diff24 - 15.9321 * diff34 - 99.4193 * contr12 - 11.2403 * contr13 + 870.5857 * contr14 + 21.2244 * contr23 + 23.7371 * contr24 + 16.1697 * contr34 + 2518.4158 ;
}

double DistanceEstimator::LM519()
{
	return 614.194 * diff12 - 1685.6851 * diff13 - 235.8424 * diff14 - 2.8392 * diff23 - 52.4775 * diff24 + 568.7154 * diff34 - 324.8023 * contr12 - 11.2403 * contr13 - 286.3454 * contr14 - 60.291 * contr23 + 5629.045 * contr24 + 911.8077 * contr34 + 1211.8483 ;
}

double DistanceEstimator::LM520()
{
	return 614.194 * diff12 - 781.2146 * diff13 - 235.8424 * diff14 - 2.8392 * diff23 - 52.4775 * diff24 + 568.7154 * diff34 - 324.8023 * contr12 - 11.2403 * contr13 - 286.3454 * contr14 - 60.291 * contr23 + 1855.1423 * contr24 + 911.8077 * contr34 + 1583.6072 ;
}

double DistanceEstimator::LM521()
{
	return 1146.022 * diff12 - 365.732 * diff13 - 235.8424 * diff14 - 2.8392 * diff23 - 52.4775 * diff24 + 987.9721 * diff34 - 324.8023 * contr12 - 11.2403 * contr13 - 286.3454 * contr14 - 60.291 * contr23 - 772.97 * contr24 + 816.0735 * contr34 + 1888.0794 ;
}

double DistanceEstimator::LM522()
{
	return 1560.7307 * diff12 - 365.732 * diff13 - 235.8424 * diff14 - 2.8392 * diff23 - 52.4775 * diff24 + 1066.8078 * diff34 - 324.8023 * contr12 - 11.2403 * contr13 - 286.3454 * contr14 - 60.291 * contr23 - 772.97 * contr24 + 816.0735 * contr34 + 1824.1588 ;
}

double DistanceEstimator::LM523()
{
	return 457.8385 * diff12 - 2346.8829 * diff13 - 221.545 * diff14 - 2.8392 * diff23 - 52.4775 * diff24 - 33.8278 * diff34 + 18.2216 * contr12 - 11.2403 * contr13 - 81.0673 * contr14 - 60.291 * contr23 - 282.3026 * contr24 + 189.3036 * contr34 + 2840.6191 ;
}

double DistanceEstimator::LM524()
{
	return 214.5425 * diff12 - 1281.0743 * diff13 - 221.545 * diff14 - 2.8392 * diff23 - 52.4775 * diff24 + 2983.0181 * diff34 - 38.9635 * contr12 - 11.2403 * contr13 - 81.0673 * contr14 - 60.291 * contr23 - 282.3026 * contr24 + 117.1509 * contr34 + 2074.8454 ;
}

double DistanceEstimator::LM525()
{
	return 214.5425 * diff12 - 1281.0743 * diff13 - 221.545 * diff14 - 2.8392 * diff23 - 52.4775 * diff24 + 2671.2222 * diff34 - 38.9635 * contr12 - 11.2403 * contr13 - 81.0673 * contr14 - 60.291 * contr23 - 282.3026 * contr24 + 117.1509 * contr34 + 2134.654 ;
}

double DistanceEstimator::LM526()
{
	return 214.5425 * diff12 - 1281.0743 * diff13 - 221.545 * diff14 - 2.8392 * diff23 - 52.4775 * diff24 + 3001.632 * diff34 - 38.9635 * contr12 - 11.2403 * contr13 - 81.0673 * contr14 - 60.291 * contr23 - 282.3026 * contr24 + 117.1509 * contr34 + 2100.2254 ;
}

double DistanceEstimator::LM527()
{
	return 214.5425 * diff12 - 2286.3323 * diff13 - 221.545 * diff14 - 2.8392 * diff23 - 52.4775 * diff24 - 33.8278 * diff34 - 38.9635 * contr12 - 11.2403 * contr13 - 81.0673 * contr14 - 60.291 * contr23 - 282.3026 * contr24 + 153.6326 * contr34 + 2911.6594 ;
}

double DistanceEstimator::LM528()
{
	return -619.5221 * diff12 - 2126.7395 * diff13 - 238.589 * diff14 - 2.8392 * diff23 - 52.4775 * diff24 - 33.8278 * diff34 - 150.4356 * contr12 - 11.2403 * contr13 - 501.1848 * contr14 - 60.291 * contr23 - 846.6483 * contr24 + 201.4673 * contr34 + 3137.8902 ;
}

double DistanceEstimator::LM529()
{
	return -984.685 * diff12 - 2126.7395 * diff13 - 238.589 * diff14 - 2.8392 * diff23 - 52.4775 * diff24 - 33.8278 * diff34 - 150.4356 * contr12 + 186.2431 * contr13 - 1632.6211 * contr14 - 60.291 * contr23 - 846.6483 * contr24 + 201.4673 * contr34 + 3238.8539 ;
}

double DistanceEstimator::LM530()
{
	return -728.5426 * diff12 - 2358.478 * diff13 - 1237.3359 * diff14 + 3909.1782 * diff23 - 52.4775 * diff24 - 33.8278 * diff34 + 1664.8901 * contr12 - 11.2403 * contr13 - 384.8435 * contr14 - 60.291 * contr23 - 927.9317 * contr24 + 201.4673 * contr34 + 2518.1735 ;
}

double DistanceEstimator::LM531()
{
	return -486.4914 * diff12 - 1798.9778 * diff13 - 238.589 * diff14 - 2.8392 * diff23 - 52.4775 * diff24 - 33.8278 * diff34 - 210.0792 * contr12 - 11.2403 * contr13 - 81.0673 * contr14 - 60.291 * contr23 - 665.1019 * contr24 + 201.4673 * contr34 + 2949.0888 ;
}

double DistanceEstimator::LM532()
{
	return -535.1373 * diff12 - 1472.1177 * diff13 + 6202.3189 * diff14 - 131.946 * diff23 - 89.5348 * diff24 + 190.1191 * diff34 - 657.2644 * contr12 - 1736.5732 * contr13 + 613.547 * contr14 - 461.3125 * contr23 + 64.9436 * contr24 - 212.714 * contr34 + 1726.596 ;
}

double DistanceEstimator::LM533()
{
	return -1043.2801 * diff12 + 509.3803 * diff13 + 1597.4964 * diff14 - 131.946 * diff23 - 89.5348 * diff24 + 190.1191 * diff34 - 1562.0409 * contr12 - 5134.6883 * contr13 + 613.547 * contr14 - 461.3125 * contr23 + 64.9436 * contr24 - 212.714 * contr34 + 3001.8306 ;
}

double DistanceEstimator::LM534()
{
	return 1195.988 * diff12 - 1036.5868 * diff13 + 456.1844 * diff14 - 131.946 * diff23 - 89.5348 * diff24 + 190.1191 * diff34 - 77.4549 * contr12 - 629.4873 * contr13 + 94.6269 * contr14 - 461.3125 * contr23 - 355.4694 * contr24 - 345.5058 * contr34 + 3172.7178 ;
}

double DistanceEstimator::LM535()
{
	return 567.6745 * diff12 - 11009.0763 * diff13 - 4645.0049 * diff14 - 63.8345 * diff23 - 89.5348 * diff24 + 483.4716 * diff34 - 1950.295 * contr12 - 112.7962 * contr13 + 986.5146 * contr14 - 701.5828 * contr23 - 410.3614 * contr24 + 94.8398 * contr34 + 6594.5445 ;
}

double DistanceEstimator::LM536()
{
	return 1017.1754 * diff12 - 7016.81 * diff13 - 1570.895 * diff14 - 63.8345 * diff23 - 89.5348 * diff24 + 483.4716 * diff34 - 982.9751 * contr12 - 1055.6491 * contr13 + 664.3055 * contr14 - 701.5828 * contr23 - 832.8656 * contr24 + 94.8398 * contr34 + 4672.8709 ;
}

double DistanceEstimator::LM537()
{
	return 1017.1754 * diff12 - 7016.81 * diff13 - 1570.895 * diff14 - 63.8345 * diff23 - 89.5348 * diff24 + 483.4716 * diff34 - 982.9751 * contr12 - 1055.6491 * contr13 + 735.2542 * contr14 - 701.5828 * contr23 - 832.8656 * contr24 + 94.8398 * contr34 + 4669.9836 ;
}

double DistanceEstimator::LM538()
{
	return 1017.1754 * diff12 - 7016.81 * diff13 - 1570.895 * diff14 - 63.8345 * diff23 - 89.5348 * diff24 + 483.4716 * diff34 - 982.9751 * contr12 - 1020.7286 * contr13 + 214.9638 * contr14 - 701.5828 * contr23 - 832.8656 * contr24 + 94.8398 * contr34 + 4682.1774 ;
}

double DistanceEstimator::LM539()
{
	return 424.5455 * diff12 - 3359.2742 * diff13 - 343.2683 * diff14 - 63.8345 * diff23 - 89.5348 * diff24 + 1750.0239 * diff34 - 253.0839 * contr12 - 112.7962 * contr13 + 249.8222 * contr14 - 701.5828 * contr23 - 196.4632 * contr24 + 2203.7442 * contr34 + 2550.0769 ;
}

double DistanceEstimator::LM540()
{
	return 1302.7243 * diff12 - 3502.2663 * diff13 + 1424.5596 * diff14 - 63.8345 * diff23 - 89.5348 * diff24 + 1475.43 * diff34 - 85.1282 * contr12 - 112.7962 * contr13 + 231.3205 * contr14 - 788.1048 * contr23 - 126.619 * contr24 + 224.5448 * contr34 + 2533.8703 ;
}

double DistanceEstimator::LM541()
{
	return -79.4714 * diff12 - 1257.043 * diff13 + 72.493 * diff14 + 1779.0721 * diff23 - 89.5348 * diff24 + 148.3084 * diff34 - 85.1282 * contr12 - 643.144 * contr13 + 349.9419 * contr14 - 1324.8882 * contr23 + 64.9436 * contr24 - 113.8758 * contr34 + 2069.7819 ;
}

double DistanceEstimator::LM542()
{
	return -79.4714 * diff12 - 1257.043 * diff13 + 72.493 * diff14 + 1564.9254 * diff23 - 89.5348 * diff24 + 148.3084 * diff34 - 85.1282 * contr12 - 643.144 * contr13 + 349.9419 * contr14 - 1324.8882 * contr23 + 64.9436 * contr24 - 113.8758 * contr34 + 2100.8704 ;
}

double DistanceEstimator::LM543()
{
	return -79.4714 * diff12 - 1257.043 * diff13 + 72.493 * diff14 + 641.8512 * diff23 - 89.5348 * diff24 + 148.3084 * diff34 + 141.3159 * contr12 - 589.0856 * contr13 + 311.0461 * contr14 - 1192.6547 * contr23 + 64.9436 * contr24 - 113.8758 * contr34 + 2120.4426 ;
}

double DistanceEstimator::LM544()
{
	return -79.4714 * diff12 - 1257.043 * diff13 + 72.493 * diff14 + 641.8512 * diff23 - 89.5348 * diff24 + 148.3084 * diff34 + 77.6285 * contr12 - 3084.5774 * contr13 + 311.0461 * contr14 - 1192.6547 * contr23 + 64.9436 * contr24 - 113.8758 * contr34 + 2946.9279 ;
}

double DistanceEstimator::LM545()
{
	return -79.4714 * diff12 - 1257.043 * diff13 + 72.493 * diff14 + 641.8512 * diff23 - 89.5348 * diff24 + 148.3084 * diff34 - 85.1282 * contr12 - 589.0856 * contr13 + 311.0461 * contr14 - 1192.6547 * contr23 + 64.9436 * contr24 - 113.8758 * contr34 + 2163.1155 ;
}

double DistanceEstimator::LM546()
{
	return -79.4714 * diff12 - 1257.043 * diff13 + 72.493 * diff14 - 300.5212 * diff23 - 89.5348 * diff24 + 148.3084 * diff34 - 1171.0197 * contr12 - 592.1685 * contr13 + 297.3985 * contr14 - 942.107 * contr23 + 64.9436 * contr24 - 113.8758 * contr34 + 2417.3595 ;
}

double DistanceEstimator::LM547()
{
	return -79.4714 * diff12 - 1257.043 * diff13 + 72.493 * diff14 + 2116.3494 * diff23 - 89.5348 * diff24 + 148.3084 * diff34 - 85.1282 * contr12 + 580.3889 * contr13 + 205.6379 * contr14 - 2179.474 * contr23 + 64.9436 * contr24 - 113.8758 * contr34 + 1732.8706 ;
}

double DistanceEstimator::LM548()
{
	return -79.4714 * diff12 - 525.2372 * diff13 + 72.493 * diff14 + 3438.8119 * diff23 - 89.5348 * diff24 + 148.3084 * diff34 + 1551.8231 * contr12 - 10435.385 * contr13 + 205.6379 * contr14 - 1824.0121 * contr23 + 64.9436 * contr24 - 113.8758 * contr34 + 4520.8529 ;
}

double DistanceEstimator::LM549()
{
	return -79.4714 * diff12 + 347.3006 * diff13 + 72.493 * diff14 + 2909.7336 * diff23 - 89.5348 * diff24 + 148.3084 * diff34 - 85.1282 * contr12 - 4955.5955 * contr13 + 205.6379 * contr14 - 4185.2508 * contr23 + 64.9436 * contr24 - 113.8758 * contr34 + 3123.5685 ;
}

double DistanceEstimator::LM550()
{
	return -79.4714 * diff12 - 9439.6938 * diff13 - 2483.8269 * diff14 - 63.8345 * diff23 - 89.5348 * diff24 + 148.3084 * diff34 + 2848.1341 * contr12 - 340.5764 * contr13 + 169.2107 * contr14 - 2787.8926 * contr23 + 64.9436 * contr24 - 113.8758 * contr34 + 4934.5877 ;
}

double DistanceEstimator::LM551()
{
	return -79.4714 * diff12 - 9439.6938 * diff13 - 2483.8269 * diff14 - 63.8345 * diff23 - 89.5348 * diff24 + 148.3084 * diff34 + 2848.1341 * contr12 - 340.5764 * contr13 + 169.2107 * contr14 - 2787.8926 * contr23 + 64.9436 * contr24 - 113.8758 * contr34 + 4929.9748 ;
}

double DistanceEstimator::LM552()
{
	return -79.4714 * diff12 - 9134.9415 * diff13 - 3136.7692 * diff14 - 63.8345 * diff23 - 89.5348 * diff24 + 148.3084 * diff34 + 1160.5037 * contr12 - 5786.1399 * contr13 + 169.2107 * contr14 - 4827.6973 * contr23 + 64.9436 * contr24 - 113.8758 * contr34 + 7373.2397 ;
}

double DistanceEstimator::LM553()
{
	return 39.3138 * diff12 + 57.5657 * diff13 + 677.1737 * diff14 + 38.2919 * diff23 + 72.9284 * diff24 - 382.9272 * diff34 - 3261.3349 * contr12 + 200.2005 * contr13 - 441.6334 * contr14 - 162.2341 * contr23 + 207.9068 * contr24 - 519.2815 * contr34 + 2477.8159 ;
}

double DistanceEstimator::LM554()
{
	return -794.8848 * diff12 + 57.5657 * diff13 + 1179.3277 * diff14 + 38.2919 * diff23 + 72.9284 * diff24 - 382.9272 * diff34 - 3261.3349 * contr12 - 277.9002 * contr13 - 1056.2901 * contr14 - 162.2341 * contr23 + 207.9068 * contr24 - 519.2815 * contr34 + 2699.7864 ;
}

double DistanceEstimator::LM555()
{
	return -1302.6579 * diff12 + 57.5657 * diff13 + 1934.0422 * diff14 + 38.2919 * diff23 + 72.9284 * diff24 - 382.9272 * diff34 - 3877.5915 * contr12 - 568.9181 * contr13 - 1106.5899 * contr14 - 162.2341 * contr23 + 207.9068 * contr24 - 519.2815 * contr34 + 2772.3754 ;
}

double DistanceEstimator::LM556()
{
	return 834.758 * diff12 + 57.5657 * diff13 - 831.8281 * diff14 + 38.2919 * diff23 + 72.9284 * diff24 - 382.9272 * diff34 - 4244.1944 * contr12 + 300.5435 * contr13 + 57.1935 * contr14 - 162.2341 * contr23 + 207.9068 * contr24 - 519.2815 * contr34 + 2495.6156 ;
}

double DistanceEstimator::LM557()
{
	return 2160.4489 * diff12 + 57.5657 * diff13 - 831.8281 * diff14 + 38.2919 * diff23 + 72.9284 * diff24 - 382.9272 * diff34 - 4244.1944 * contr12 + 300.5435 * contr13 + 356.4896 * contr14 - 162.2341 * contr23 + 207.9068 * contr24 - 519.2815 * contr34 + 2282.6275 ;
}

double DistanceEstimator::LM558()
{
	return 2210.3563 * diff12 + 57.5657 * diff13 - 831.8281 * diff14 + 38.2919 * diff23 + 72.9284 * diff24 - 382.9272 * diff34 - 4244.1944 * contr12 + 300.5435 * contr13 + 356.4896 * contr14 - 162.2341 * contr23 + 207.9068 * contr24 - 519.2815 * contr34 + 2283.6151 ;
}

double DistanceEstimator::LM559()
{
	return 39.3138 * diff12 + 57.5657 * diff13 - 1744.3256 * diff14 + 464.2915 * diff23 + 72.9284 * diff24 - 382.9272 * diff34 - 653.7131 * contr12 + 914.4545 * contr13 - 204.4599 * contr14 - 3364.517 * contr23 + 207.9068 * contr24 - 2708.6865 * contr34 + 2908.7897 ;
}

double DistanceEstimator::LM560()
{
	return 39.3138 * diff12 + 57.5657 * diff13 - 4193.8788 * diff14 + 464.2915 * diff23 + 72.9284 * diff24 - 382.9272 * diff34 - 653.7131 * contr12 + 524.3787 * contr13 - 204.4599 * contr14 - 2816.9988 * contr23 + 207.9068 * contr24 - 2708.6865 * contr34 + 3364.4027 ;
}

double DistanceEstimator::LM561()
{
	return 39.3138 * diff12 + 57.5657 * diff13 - 4725.409 * diff14 + 464.2915 * diff23 + 72.9284 * diff24 - 382.9272 * diff34 - 653.7131 * contr12 + 914.4545 * contr13 - 204.4599 * contr14 - 900.685 * contr23 + 207.9068 * contr24 - 4211.7902 * contr34 + 3704.0576 ;
}

double DistanceEstimator::LM562()
{
	return 39.3138 * diff12 + 57.5657 * diff13 - 4725.409 * diff14 + 464.2915 * diff23 + 72.9284 * diff24 - 382.9272 * diff34 - 653.7131 * contr12 + 914.4545 * contr13 - 204.4599 * contr14 - 900.685 * contr23 + 207.9068 * contr24 - 4303.7939 * contr34 + 3741.6615 ;
}

double DistanceEstimator::LM563()
{
	return 39.3138 * diff12 + 57.5657 * diff13 - 4725.409 * diff14 + 464.2915 * diff23 + 72.9284 * diff24 - 382.9272 * diff34 - 653.7131 * contr12 + 914.4545 * contr13 - 204.4599 * contr14 - 900.685 * contr23 + 207.9068 * contr24 - 4293.3773 * contr34 + 3737.2052 ;
}

double DistanceEstimator::LM564()
{
	return 39.3138 * diff12 + 57.5657 * diff13 - 2125.2058 * diff14 - 656.2419 * diff23 + 72.9284 * diff24 - 382.9272 * diff34 + 817.0755 * contr12 + 391.5289 * contr13 - 204.4599 * contr14 - 413.9787 * contr23 + 207.9068 * contr24 - 567.0028 * contr34 + 2461.6166 ;
}

double DistanceEstimator::LM565()
{
	return 1013.1517 * diff12 - 692.0523 * diff13 - 744.9542 * diff14 + 574.216 * diff23 + 72.9284 * diff24 - 1630.1637 * diff34 - 371.493 * contr12 + 266.6794 * contr13 + 104.1715 * contr23 + 1092.6904 * contr24 - 10.6608 * contr34 + 2221.5312 ;
}

double DistanceEstimator::LM566()
{
	return 998.8653 * diff12 - 692.0523 * diff13 - 162.5695 * diff14 + 574.216 * diff23 + 72.9284 * diff24 - 1630.1637 * diff34 - 371.493 * contr12 + 325.7978 * contr13 + 104.1715 * contr23 + 1092.6904 * contr24 - 391.4509 * contr34 + 2241.0493 ;
}

double DistanceEstimator::LM567()
{
	return 392.9187 * diff12 - 692.0523 * diff13 + 288.8278 * diff14 + 574.216 * diff23 + 72.9284 * diff24 - 1630.1637 * diff34 - 371.493 * contr12 + 325.7978 * contr13 + 104.1715 * contr23 + 1092.6904 * contr24 - 277.2139 * contr34 + 2207.8765 ;
}

double DistanceEstimator::LM568()
{
	return -178.7605 * diff12 - 692.0523 * diff13 - 1365.4978 * diff14 + 574.216 * diff23 + 72.9284 * diff24 - 1630.1637 * diff34 - 371.493 * contr12 + 430.3919 * contr13 - 85.7649 * contr14 + 104.1715 * contr23 + 1092.6904 * contr24 - 10.6608 * contr34 + 2213.0261 ;
}

double DistanceEstimator::LM569()
{
	return -178.7605 * diff12 - 692.0523 * diff13 - 1365.4978 * diff14 + 574.216 * diff23 + 72.9284 * diff24 - 1630.1637 * diff34 - 371.493 * contr12 + 430.3919 * contr13 - 85.7649 * contr14 + 104.1715 * contr23 + 1092.6904 * contr24 - 10.6608 * contr34 + 2211.9775 ;
}

double DistanceEstimator::LM570()
{
	return -165.7664 * diff12 - 692.0523 * diff13 - 1365.4978 * diff14 + 574.216 * diff23 + 72.9284 * diff24 - 1630.1637 * diff34 - 371.493 * contr12 + 430.3919 * contr13 - 99.3067 * contr14 + 104.1715 * contr23 + 1092.6904 * contr24 - 10.6608 * contr34 + 2206.5107 ;
}

double DistanceEstimator::LM571()
{
	return 1067.8485 * diff12 - 692.0523 * diff13 - 1365.4978 * diff14 + 574.216 * diff23 + 72.9284 * diff24 - 1630.1637 * diff34 + 588.2352 * contr12 + 364.4179 * contr13 + 104.1715 * contr23 + 1092.6904 * contr24 - 10.6608 * contr34 + 2139.7253 ;
}

double DistanceEstimator::LM572()
{
	return 1067.8485 * diff12 - 692.0523 * diff13 - 1365.4978 * diff14 + 574.216 * diff23 + 72.9284 * diff24 - 1630.1637 * diff34 + 563.1374 * contr12 + 364.4179 * contr13 + 104.1715 * contr23 + 1092.6904 * contr24 - 10.6608 * contr34 + 2147.1872 ;
}

double DistanceEstimator::LM573()
{
	return 702.8846 * diff12 - 692.0523 * diff13 - 1365.4978 * diff14 + 574.216 * diff23 + 72.9284 * diff24 - 1630.1637 * diff34 - 43.7721 * contr12 + 364.4179 * contr13 + 104.1715 * contr23 + 1092.6904 * contr24 - 10.6608 * contr34 + 2257.3985 ;
}

double DistanceEstimator::LM574()
{
	return -1089.2794 * diff12 - 1193.296 * diff13 - 1892.1774 * diff14 + 1027.2725 * diff23 + 72.9284 * diff24 - 2503.5723 * diff34 - 2587.8734 * contr12 + 190.0654 * contr13 - 30.7612 * contr23 + 2941.3307 * contr24 - 10.6608 * contr34 + 2559.3096 ;
}

double DistanceEstimator::LM575()
{
	return -1089.2794 * diff12 - 1193.296 * diff13 - 1892.1774 * diff14 + 1027.2725 * diff23 + 72.9284 * diff24 - 2503.5723 * diff34 - 2033.7783 * contr12 + 80.9113 * contr13 - 30.7612 * contr23 + 2941.3307 * contr24 - 10.6608 * contr34 + 2471.49 ;
}

double DistanceEstimator::LM576()
{
	return -653.232 * diff12 - 1193.296 * diff13 - 1892.1774 * diff14 + 1027.2725 * diff23 + 72.9284 * diff24 - 2503.5723 * diff34 + 607.8967 * contr12 + 735.4603 * contr13 - 2275.7096 * contr14 - 30.7612 * contr23 + 2644.8848 * contr24 - 10.6608 * contr34 + 2722.5392 ;
}

double DistanceEstimator::LM577()
{
	return 39.3138 * diff12 - 984.1254 * diff13 - 2785.6633 * diff14 + 873.317 * diff23 + 72.9284 * diff24 - 2974.9792 * diff34 - 371.493 * contr12 + 437.0858 * contr13 - 276.8619 * contr14 - 30.7612 * contr23 + 1843.6749 * contr24 - 10.6608 * contr34 + 2720.8723 ;
}

double DistanceEstimator::LM578()
{
	return 39.3138 * diff12 - 984.1254 * diff13 - 2920.5682 * diff14 + 873.317 * diff23 + 72.9284 * diff24 - 2974.9792 * diff34 - 371.493 * contr12 + 977.4251 * contr13 - 276.8619 * contr14 - 30.7612 * contr23 + 1843.6749 * contr24 - 10.6608 * contr34 + 2627.3027 ;
}

double DistanceEstimator::LM579()
{
	return 39.3138 * diff12 - 1303.798 * diff13 - 2942.8103 * diff14 + 1028.5461 * diff23 + 72.9284 * diff24 - 3297.2835 * diff34 - 956.8221 * contr12 + 555.6284 * contr13 - 210.7423 * contr14 - 299.4166 * contr23 + 1843.6749 * contr24 - 10.6608 * contr34 + 2845.5199 ;
}

double DistanceEstimator::LM580()
{
	return 39.3138 * diff12 - 1060.4731 * diff13 - 2130.3662 * diff14 + 1412.5167 * diff23 + 72.9284 * diff24 - 2959.8532 * diff34 - 956.8221 * contr12 + 555.6284 * contr13 - 210.7423 * contr14 - 299.4166 * contr23 + 1843.6749 * contr24 - 10.6608 * contr34 + 2504.36 ;
}

double DistanceEstimator::LM581()
{
	return 39.3138 * diff12 - 478.4124 * diff13 - 2217.3892 * diff14 + 572.6995 * diff23 + 72.9284 * diff24 - 2503.3298 * diff34 - 956.8221 * contr12 + 555.6284 * contr13 - 210.7423 * contr14 - 299.4166 * contr23 + 1843.6749 * contr24 - 10.6608 * contr34 + 2389.931 ;
}

double DistanceEstimator::LM582()
{
	return 39.3138 * diff12 - 984.1254 * diff13 - 1943.6997 * diff14 + 238.68 * diff23 + 72.9284 * diff24 - 2503.3298 * diff34 - 4401.5323 * contr12 + 555.6284 * contr13 - 585.6135 * contr14 - 597.9226 * contr23 + 1843.6749 * contr24 - 10.6608 * contr34 + 3290.3658 ;
}

double DistanceEstimator::LM583()
{
	return 39.3138 * diff12 - 984.1254 * diff13 - 1943.6997 * diff14 + 238.68 * diff23 + 72.9284 * diff24 - 2503.3298 * diff34 - 5030.4227 * contr12 + 555.6284 * contr13 - 612.614 * contr14 - 597.9226 * contr23 + 1843.6749 * contr24 - 10.6608 * contr34 + 3370.4965 ;
}

double DistanceEstimator::LM584()
{
	return 39.3138 * diff12 - 984.1254 * diff13 - 1943.6997 * diff14 + 238.68 * diff23 + 72.9284 * diff24 - 2503.3298 * diff34 - 5050.9789 * contr12 + 555.6284 * contr13 - 612.614 * contr14 - 597.9226 * contr23 + 1843.6749 * contr24 - 10.6608 * contr34 + 3371.7144 ;
}

double DistanceEstimator::LM585()
{
	return 39.3138 * diff12 + 729.4843 * diff13 - 1686.1315 * diff14 + 995.0467 * diff23 + 72.9284 * diff24 - 2241.8927 * diff34 - 371.493 * contr12 + 1090.7328 * contr13 - 30.7612 * contr23 + 2056.9148 * contr24 + 523.1711 * contr34 + 1300.6663 ;
}

double DistanceEstimator::LM586()
{
	return 39.3138 * diff12 - 186.7582 * diff13 - 1261.5344 * diff14 + 118.9987 * diff23 + 72.9284 * diff24 + 1107.5737 * diff34 - 344.0908 * contr12 + 1195.9805 * contr13 - 389.3794 * contr23 + 200.5645 * contr24 - 10.6608 * contr34 + 1861.0024 ;
}

double DistanceEstimator::LM587()
{
	return 39.3138 * diff12 - 4904.1102 * diff13 - 2516.2952 * diff14 + 118.9987 * diff23 + 72.9284 * diff24 - 527.4568 * diff34 - 344.0908 * contr12 + 5792.7089 * contr13 - 389.3794 * contr23 + 200.5645 * contr24 - 10.6608 * contr34 + 2460.1593 ;
}

double DistanceEstimator::LM588()
{
	return 39.3138 * diff12 - 4792.7547 * diff13 - 2516.2952 * diff14 + 118.9987 * diff23 + 72.9284 * diff24 - 527.4568 * diff34 - 344.0908 * contr12 + 5792.7089 * contr13 - 389.3794 * contr23 + 200.5645 * contr24 - 10.6608 * contr34 + 2417.0017 ;
}

double DistanceEstimator::LM589()
{
	return 39.3138 * diff12 - 2013.6868 * diff13 - 2516.2952 * diff14 + 118.9987 * diff23 + 72.9284 * diff24 - 527.4568 * diff34 - 344.0908 * contr12 + 4357.9629 * contr13 - 389.3794 * contr23 + 200.5645 * contr24 - 10.6608 * contr34 + 1982.8547 ;
}

double DistanceEstimator::LM590()
{
	return 601.6372 * diff12 + 4882.7629 * diff13 - 118.232 * diff14 + 906.2862 * diff23 + 72.9284 * diff24 - 1651.449 * diff34 + 1605.0964 * contr12 - 389.8626 * contr13 + 413.9009 * contr14 + 2497.1555 * contr23 + 43.1621 * contr24 - 138.8714 * contr34 - 308.3611 ;
}

double DistanceEstimator::LM591()
{
	return 403.7827 * diff12 + 4882.7629 * diff13 - 118.232 * diff14 + 906.2862 * diff23 + 72.9284 * diff24 - 540.888 * diff34 + 2925.7687 * contr12 - 389.8626 * contr13 + 413.9009 * contr14 + 1731.0613 * contr23 + 43.1621 * contr24 - 138.8714 * contr34 - 604.4926 ;
}

double DistanceEstimator::LM592()
{
	return 403.7827 * diff12 + 4882.7629 * diff13 - 118.232 * diff14 + 906.2862 * diff23 + 72.9284 * diff24 + 118.2107 * diff34 + 2077.3713 * contr12 - 389.8626 * contr13 + 413.9009 * contr14 + 1731.0613 * contr23 + 43.1621 * contr24 - 138.8714 * contr34 - 486.4127 ;
}

double DistanceEstimator::LM593()
{
	return 403.7827 * diff12 + 4882.7629 * diff13 - 118.232 * diff14 + 906.2862 * diff23 + 72.9284 * diff24 + 118.2107 * diff34 + 2077.3713 * contr12 - 389.8626 * contr13 + 413.9009 * contr14 + 1731.0613 * contr23 + 43.1621 * contr24 - 138.8714 * contr34 - 484.3345 ;
}

double DistanceEstimator::LM594()
{
	return 403.7827 * diff12 + 4882.7629 * diff13 - 118.232 * diff14 + 906.2862 * diff23 + 72.9284 * diff24 + 118.2107 * diff34 + 2077.3713 * contr12 - 389.8626 * contr13 + 413.9009 * contr14 + 2646.3324 * contr23 + 43.1621 * contr24 - 138.8714 * contr34 - 553.0095 ;
}

double DistanceEstimator::LM595()
{
	return 403.7827 * diff12 + 4882.7629 * diff13 - 118.232 * diff14 + 906.2862 * diff23 + 72.9284 * diff24 + 118.2107 * diff34 + 2077.3713 * contr12 - 389.8626 * contr13 + 413.9009 * contr14 + 1731.0613 * contr23 + 43.1621 * contr24 - 138.8714 * contr34 - 492.5908 ;
}

double DistanceEstimator::LM596()
{
	return 403.7827 * diff12 + 4882.7629 * diff13 - 118.232 * diff14 + 906.2862 * diff23 + 72.9284 * diff24 + 817.0383 * diff34 + 2586.3734 * contr12 - 389.8626 * contr13 + 413.9009 * contr14 + 1731.0613 * contr23 + 43.1621 * contr24 - 138.8714 * contr34 - 653.0131 ;
}

double DistanceEstimator::LM597()
{
	return 39.3138 * diff12 + 830.0399 * diff13 - 118.232 * diff14 - 1778.9215 * diff23 + 72.9284 * diff24 - 1625.1996 * diff34 - 740.419 * contr12 - 389.8626 * contr13 + 406.2932 * contr14 + 319.8353 * contr23 + 43.1621 * contr24 - 138.8714 * contr34 + 2271.2497 ;
}

double DistanceEstimator::LM598()
{
	return -206.354 * diff12 - 3466.8716 * diff13 - 118.232 * diff14 - 2265.2533 * diff23 + 72.9284 * diff24 - 1625.1996 * diff34 - 2247.2706 * contr12 - 389.8626 * contr13 + 406.2932 * contr14 + 319.8353 * contr23 + 43.1621 * contr24 - 138.8714 * contr34 + 4099.6485 ;
}

double DistanceEstimator::LM599()
{
	return 134.4334 * diff12 - 3466.8716 * diff13 - 118.232 * diff14 - 2265.2533 * diff23 + 72.9284 * diff24 - 1625.1996 * diff34 - 1985.2094 * contr12 - 389.8626 * contr13 + 406.2932 * contr14 + 319.8353 * contr23 + 43.1621 * contr24 - 138.8714 * contr34 + 3980.2532 ;
}

double DistanceEstimator::LM600()
{
	return 39.3138 * diff12 - 1877.9876 * diff13 - 118.232 * diff14 - 1652.0966 * diff23 + 72.9284 * diff24 - 1625.1996 * diff34 - 740.419 * contr12 + 324.022 * contr13 + 406.2932 * contr14 + 319.8353 * contr23 + 43.1621 * contr24 - 138.8714 * contr34 + 2938.4461 ;
}

double DistanceEstimator::LM601()
{
	return 39.3138 * diff12 - 1877.9876 * diff13 - 118.232 * diff14 - 1652.0966 * diff23 + 72.9284 * diff24 - 1625.1996 * diff34 + 2984.5414 * contr12 - 2223.9788 * contr13 + 406.2932 * contr14 + 319.8353 * contr23 + 43.1621 * contr24 - 138.8714 * contr34 + 2907.7688 ;
}

double DistanceEstimator::LM602()
{
	return 39.3138 * diff12 - 1877.9876 * diff13 - 118.232 * diff14 - 1652.0966 * diff23 + 72.9284 * diff24 - 1625.1996 * diff34 + 2984.5414 * contr12 - 2134.7708 * contr13 + 406.2932 * contr14 + 319.8353 * contr23 + 43.1621 * contr24 - 138.8714 * contr34 + 2883.3572 ;
}

double DistanceEstimator::LM603()
{
	return 39.3138 * diff12 - 1877.9876 * diff13 - 118.232 * diff14 - 1921.6085 * diff23 + 72.9284 * diff24 - 1625.1996 * diff34 + 2822.5866 * contr12 - 1550.5414 * contr13 + 406.2932 * contr14 + 319.8353 * contr23 + 43.1621 * contr24 - 138.8714 * contr34 + 2813.0857 ;
}

double DistanceEstimator::LM604()
{
	return -2055.6369 * diff12 + 5930.1046 * diff13 - 118.232 * diff14 + 2131.8125 * diff23 + 72.9284 * diff24 - 1685.1175 * diff34 - 1614.3483 * contr12 - 389.8626 * contr13 + 351.0287 * contr14 + 1048.6216 * contr23 + 43.1621 * contr24 - 138.8714 * contr34 + 458.4763 ;
}

double DistanceEstimator::LM605()
{
	return 1516.2974 * diff12 + 2978.4811 * diff13 - 118.232 * diff14 + 5317.877 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 258.2511 * contr12 - 545.7994 * contr13 + 5748.8902 * contr23 + 43.1621 * contr24 + 22.1429 * contr34 - 1124.672 ;
}

double DistanceEstimator::LM606()
{
	return 3589.584 * diff12 + 2978.4811 * diff13 - 118.232 * diff14 + 5317.877 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 258.2511 * contr12 - 545.7994 * contr13 + 6790.3912 * contr23 + 43.1621 * contr24 + 22.1429 * contr34 - 1513.6222 ;
}

double DistanceEstimator::LM607()
{
	return 3868.982 * diff12 + 2978.4811 * diff13 - 118.232 * diff14 + 8104.1933 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 258.2511 * contr12 - 545.7994 * contr13 + 7557.0541 * contr23 + 43.1621 * contr24 + 22.1429 * contr34 - 2146.6532 ;
}

double DistanceEstimator::LM608()
{
	return -615.5299 * diff12 + 2978.4811 * diff13 - 118.232 * diff14 + 1887.607 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 258.2511 * contr12 - 545.7994 * contr13 - 91.2245 * contr14 + 229.8 * contr23 + 43.1621 * contr24 - 30.0166 * contr34 + 624.0225 ;
}

double DistanceEstimator::LM609()
{
	return -623.1178 * diff12 + 2978.4811 * diff13 - 118.232 * diff14 + 1887.607 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 258.2511 * contr12 - 545.7994 * contr13 - 44.0394 * contr14 + 229.8 * contr23 + 43.1621 * contr24 - 30.0166 * contr34 + 651.6352 ;
}

double DistanceEstimator::LM610()
{
	return -1035.1734 * diff12 + 4575.2833 * diff13 - 118.232 * diff14 + 1887.607 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 258.2511 * contr12 - 545.7994 * contr13 - 133.4566 * contr14 + 653.0926 * contr23 + 43.1621 * contr24 - 30.0166 * contr34 + 142.2205 ;
}

double DistanceEstimator::LM611()
{
	return -576.8721 * diff12 + 3896.6423 * diff13 - 118.232 * diff14 + 1887.607 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 258.2511 * contr12 - 545.7994 * contr13 - 177.0495 * contr14 + 766.0758 * contr23 + 43.1621 * contr24 - 30.0166 * contr34 + 281.2752 ;
}

double DistanceEstimator::LM612()
{
	return -429.244 * diff12 + 3896.6423 * diff13 - 118.232 * diff14 + 1887.607 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 258.2511 * contr12 - 545.7994 * contr13 - 158.6248 * contr14 + 919.1612 * contr23 + 43.1621 * contr24 - 30.0166 * contr34 + 234.1815 ;
}

double DistanceEstimator::LM613()
{
	return -2944.2202 * diff12 + 10056.5167 * diff13 - 118.232 * diff14 + 1511.8166 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 2902.8974 * contr12 + 64.2827 * contr13 - 82.2537 * contr23 + 43.1621 * contr24 - 250.4815 * contr34 - 656.922 ;
}

double DistanceEstimator::LM614()
{
	return -3896.5772 * diff12 + 6721.2562 * diff13 - 118.232 * diff14 + 1511.8166 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 2280.7515 * contr12 + 844.7062 * contr13 - 82.2537 * contr23 + 43.1621 * contr24 + 848.3961 * contr34 - 195.7776 ;
}

double DistanceEstimator::LM615()
{
	return -3614.6039 * diff12 + 6721.2562 * diff13 - 118.232 * diff14 + 1511.8166 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 2280.7515 * contr12 + 939.8491 * contr13 - 82.2537 * contr23 + 43.1621 * contr24 + 848.3961 * contr34 - 260.8318 ;
}

double DistanceEstimator::LM616()
{
	return -4264.0359 * diff12 + 5863.9646 * diff13 - 118.232 * diff14 + 1511.8166 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 2280.7515 * contr12 + 1105.537 * contr13 - 82.2537 * contr23 + 43.1621 * contr24 + 848.3961 * contr34 + 17.9093 ;
}

double DistanceEstimator::LM617()
{
	return -9211.8423 * diff12 + 8238.003 * diff13 - 118.232 * diff14 + 1511.8166 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 2280.7515 * contr12 + 5136.4748 * contr13 - 82.2537 * contr23 + 43.1621 * contr24 + 1764.1273 * contr34 - 1618.7568 ;
}

double DistanceEstimator::LM618()
{
	return -9018.5983 * diff12 + 8238.003 * diff13 - 118.232 * diff14 + 1511.8166 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 2280.7515 * contr12 + 5136.4748 * contr13 - 82.2537 * contr23 + 43.1621 * contr24 + 1764.1273 * contr34 - 1666.6102 ;
}

double DistanceEstimator::LM619()
{
	return 39.3138 * diff12 + 1088.1357 * diff13 - 3023.9538 * diff14 + 228.3334 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 174.3092 * contr12 - 1212.5621 * contr13 - 82.2537 * contr23 + 43.1621 * contr24 - 554.5845 * contr34 + 2660.9726 ;
}

double DistanceEstimator::LM620()
{
	return 39.3138 * diff12 + 1088.1357 * diff13 - 1449.4904 * diff14 + 228.3334 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 174.3092 * contr12 - 6007.3604 * contr13 - 82.2537 * contr23 + 43.1621 * contr24 - 554.5845 * contr34 + 3792.5741 ;
}

double DistanceEstimator::LM621()
{
	return 39.3138 * diff12 + 1088.1357 * diff13 - 2038.2256 * diff14 + 228.3334 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 174.3092 * contr12 + 3727.4921 * contr13 - 82.2537 * contr23 + 43.1621 * contr24 - 1083.6095 * contr34 + 990.367 ;
}

double DistanceEstimator::LM622()
{
	return 39.3138 * diff12 + 1088.1357 * diff13 - 2038.2256 * diff14 + 228.3334 * diff23 + 72.9284 * diff24 - 85.0646 * diff34 - 174.3092 * contr12 + 4973.9185 * contr13 - 82.2537 * contr23 + 43.1621 * contr24 - 1083.6095 * contr34 + 738.6658 ;
}

double DistanceEstimator::LM623()
{
	return 39.3138 * diff12 + 231.0361 * diff13 - 105.1798 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 299.6049 * diff34 - 988.2379 * contr12 + 381.3931 * contr13 - 240.263 * contr23 + 4.111 * contr24 + 242.689 * contr34 + 1948.8006 ;
}

double DistanceEstimator::LM624()
{
	return 39.3138 * diff12 + 231.0361 * diff13 - 228.8082 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 299.6049 * diff34 - 988.2379 * contr12 + 381.3931 * contr13 - 106.495 * contr23 + 4.111 * contr24 + 169.3959 * contr34 + 1901.734 ;
}

double DistanceEstimator::LM625()
{
	return 39.3138 * diff12 + 231.0361 * diff13 - 216.7153 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 299.6049 * diff34 - 988.2379 * contr12 + 381.3931 * contr13 - 106.495 * contr23 + 4.111 * contr24 + 169.3959 * contr34 + 1902.3149 ;
}

double DistanceEstimator::LM626()
{
	return 39.3138 * diff12 + 231.0361 * diff13 - 458.5726 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 299.6049 * diff34 - 988.2379 * contr12 + 381.3931 * contr13 - 106.495 * contr23 + 4.111 * contr24 + 169.3959 * contr34 + 1944.8907 ;
}

double DistanceEstimator::LM627()
{
	return 39.3138 * diff12 + 231.0361 * diff13 - 582.9327 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 299.6049 * diff34 - 988.2379 * contr12 + 381.3931 * contr13 - 57.2301 * contr23 + 4.111 * contr24 + 147.6794 * contr34 + 1980.549 ;
}

double DistanceEstimator::LM628()
{
	return 542.0086 * diff12 - 2400.6935 * diff13 + 63.2518 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 133.426 * diff34 - 1006.8889 * contr12 + 186.9381 * contr13 + 164.8003 * contr14 + 618.6757 * contr23 + 338.8897 * contr24 + 475.1605 * contr34 + 2282.5079 ;
}

double DistanceEstimator::LM629()
{
	return 542.0086 * diff12 - 2607.5392 * diff13 + 63.2518 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 133.426 * diff34 - 1006.8889 * contr12 + 186.9381 * contr13 + 164.8003 * contr14 + 603.3998 * contr23 + 338.8897 * contr24 + 475.1605 * contr34 + 2340.6432 ;
}

double DistanceEstimator::LM630()
{
	return 542.0086 * diff12 - 2544.8549 * diff13 + 63.2518 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 133.426 * diff34 - 1006.8889 * contr12 + 186.9381 * contr13 + 164.8003 * contr14 + 595.3043 * contr23 + 338.8897 * contr24 + 475.1605 * contr34 + 2322.1922 ;
}

double DistanceEstimator::LM631()
{
	return 64.2558 * diff12 - 1017.0761 * diff13 + 63.2518 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 133.426 * diff34 - 1006.8889 * contr12 + 186.9381 * contr13 + 164.8003 * contr14 + 704.4338 * contr23 + 338.8897 * contr24 + 475.1605 * contr34 + 1931.5894 ;
}

double DistanceEstimator::LM632()
{
	return -22.6083 * diff12 - 1017.0761 * diff13 + 63.2518 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 133.426 * diff34 - 1006.8889 * contr12 + 186.9381 * contr13 + 164.8003 * contr14 + 704.4338 * contr23 + 338.8897 * contr24 + 475.1605 * contr34 + 1933.1997 ;
}

double DistanceEstimator::LM633()
{
	return 537.8582 * diff12 + 90.9214 * diff13 + 63.2518 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 133.426 * diff34 - 1006.8889 * contr12 + 186.9381 * contr13 + 161.0548 * contr14 + 441.7008 * contr23 + 2906.7457 * contr24 + 475.1605 * contr34 + 1212.161 ;
}

double DistanceEstimator::LM634()
{
	return -4587.0169 * diff12 + 90.9214 * diff13 + 63.2518 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 133.426 * diff34 + 316.7823 * contr12 + 186.9381 * contr13 + 1973.711 * contr23 + 331.3779 * contr24 + 475.1605 * contr34 + 1788.3907 ;
}

double DistanceEstimator::LM635()
{
	return -4433.8317 * diff12 + 90.9214 * diff13 + 63.2518 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 133.426 * diff34 + 316.7823 * contr12 + 186.9381 * contr13 + 1973.711 * contr23 + 331.3779 * contr24 + 475.1605 * contr34 + 1747.337 ;
}

double DistanceEstimator::LM636()
{
	return 1269.801 * diff12 + 90.9214 * diff13 + 63.2518 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 133.426 * diff34 - 1541.2171 * contr12 + 186.9381 * contr13 + 998.0026 * contr23 + 331.3779 * contr24 + 475.1605 * contr34 + 1706.8364 ;
}

double DistanceEstimator::LM637()
{
	return 804.3745 * diff12 + 90.9214 * diff13 + 63.2518 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 133.426 * diff34 - 506.3307 * contr12 + 186.9381 * contr13 + 998.0026 * contr23 + 331.3779 * contr24 + 475.1605 * contr34 + 1589.1473 ;
}

double DistanceEstimator::LM638()
{
	return 3937.088 * diff12 + 90.9214 * diff13 + 63.2518 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 133.426 * diff34 - 2200.6595 * contr12 + 186.9381 * contr13 - 1339.23 * contr23 - 612.9012 * contr24 + 977.9538 * contr34 + 1947.0595 ;
}

double DistanceEstimator::LM639()
{
	return 2934.4021 * diff12 + 90.9214 * diff13 + 63.2518 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 133.426 * diff34 - 2200.6595 * contr12 - 2035.404 * contr13 - 1339.23 * contr23 - 612.9012 * contr24 + 977.9538 * contr34 + 2681.4566 ;
}

double DistanceEstimator::LM640()
{
	return -178.9329 * diff12 - 948.4911 * diff13 - 561.0622 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 170.9913 * diff34 - 798.6307 * contr12 - 94.0041 * contr13 - 216.2073 * contr23 + 2664.7953 * contr24 + 321.9341 * contr34 + 1895.7039 ;
}

double DistanceEstimator::LM641()
{
	return -187.9223 * diff12 - 1422.2033 * diff13 - 336.0245 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 1149.0669 * diff34 - 798.6307 * contr12 - 521.0394 * contr13 - 216.2073 * contr23 + 2249.1803 * contr24 + 321.9341 * contr34 + 1851.9978 ;
}

double DistanceEstimator::LM642()
{
	return -208.7904 * diff12 - 1003.8309 * diff13 - 367.8821 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 170.9913 * diff34 - 798.6307 * contr12 - 628.3144 * contr13 - 216.2073 * contr23 + 2343.4917 * contr24 + 321.9341 * contr34 + 2014.4575 ;
}

double DistanceEstimator::LM643()
{
	return 39.3138 * diff12 - 745.9674 * diff13 - 108.4113 * diff14 + 6.9649 * diff23 + 118.1328 * diff24 + 170.9913 * diff34 - 798.6307 * contr12 + 186.9381 * contr13 - 216.2073 * contr23 + 1357.9197 * contr24 + 321.9341 * contr34 + 1916.8007 ;
}

double DistanceEstimator::LM644()
{
	return 956.0649 * diff12 + 170.5835 * diff13 - 20.2274 * diff14 + 147.5943 * diff23 + 280.1376 * diff24 - 0.4775 * diff34 + 777.3246 * contr12 + 6.757 * contr13 + 1824.6936 * contr14 - 14.6062 * contr23 - 1179.267 * contr24 + 3487.344 * contr34 + 777.9161 ;
}

double DistanceEstimator::LM645()
{
	return 632.9712 * diff12 + 170.5835 * diff13 - 20.2274 * diff14 + 147.5943 * diff23 + 280.1376 * diff24 - 0.4775 * diff34 + 1646.5426 * contr12 + 6.757 * contr13 + 591.6324 * contr14 - 14.6062 * contr23 - 1133.7524 * contr24 + 3428.6394 * contr34 + 1042.3279 ;
}

double DistanceEstimator::LM646()
{
	return 483.3526 * diff12 + 170.5835 * diff13 - 20.2274 * diff14 + 147.5943 * diff23 + 280.1376 * diff24 - 334.5971 * diff34 + 746.4839 * contr12 + 6.757 * contr13 + 2822.1559 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 1872.9629 * contr34 + 526.3484 ;
}

double DistanceEstimator::LM647()
{
	return 791.9329 * diff12 + 170.5835 * diff13 - 20.2274 * diff14 + 147.5943 * diff23 + 280.1376 * diff24 - 169.3054 * diff34 + 746.4839 * contr12 + 6.757 * contr13 + 3501.0108 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 1872.9629 * contr34 + 290.5544 ;
}

double DistanceEstimator::LM648()
{
	return -353.1101 * diff12 - 608.9473 * diff13 - 20.2274 * diff14 + 147.5943 * diff23 + 280.1376 * diff24 + 67.8889 * diff34 - 597.639 * contr12 + 6.757 * contr13 + 133.7125 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 1586.2262 * contr34 + 1611.9982 ;
}

double DistanceEstimator::LM649()
{
	return -353.1101 * diff12 - 228.393 * diff13 - 20.2274 * diff14 + 147.5943 * diff23 + 280.1376 * diff24 + 67.8889 * diff34 - 208.1801 * contr12 - 69.9194 * contr13 + 570.2875 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 248.8319 * contr34 + 1622.435 ;
}

double DistanceEstimator::LM650()
{
	return -353.1101 * diff12 - 228.393 * diff13 - 20.2274 * diff14 + 147.5943 * diff23 + 280.1376 * diff24 + 67.8889 * diff34 - 208.1801 * contr12 - 63.5297 * contr13 + 570.2875 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 248.8319 * contr34 + 1617.9488 ;
}

double DistanceEstimator::LM651()
{
	return -353.1101 * diff12 + 48.3086 * diff13 - 20.2274 * diff14 + 147.5943 * diff23 + 280.1376 * diff24 + 67.8889 * diff34 + 132.8766 * contr12 + 6.757 * contr13 + 363.0994 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 248.8319 * contr34 + 1569.3435 ;
}

double DistanceEstimator::LM652()
{
	return -353.1101 * diff12 + 90.7304 * diff13 - 20.2274 * diff14 + 147.5943 * diff23 + 280.1376 * diff24 + 67.8889 * diff34 + 409.7509 * contr12 + 6.757 * contr13 - 59.8922 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 248.8319 * contr34 + 1641.386 ;
}

double DistanceEstimator::LM653()
{
	return -353.1101 * diff12 + 13.4813 * diff13 - 20.2274 * diff14 + 147.5943 * diff23 + 280.1376 * diff24 + 67.8889 * diff34 + 221.4142 * contr12 + 6.757 * contr13 - 132.7405 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 248.8319 * contr34 + 1724.8691 ;
}

double DistanceEstimator::LM654()
{
	return -353.1101 * diff12 + 13.4813 * diff13 - 20.2274 * diff14 + 147.5943 * diff23 + 280.1376 * diff24 + 67.8889 * diff34 + 221.4142 * contr12 + 6.757 * contr13 - 45.1282 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 248.8319 * contr34 + 1693.6607 ;
}

double DistanceEstimator::LM655()
{
	return -1061.5492 * diff12 - 1770.1127 * diff13 - 589.5831 * diff14 + 147.5943 * diff23 + 543.132 * diff24 + 54.0957 * diff34 + 66.2114 * contr12 + 6.757 * contr13 - 1102.8699 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 472.9379 * contr34 + 2576.0778 ;
}

double DistanceEstimator::LM656()
{
	return -1061.5492 * diff12 + 93.0741 * diff13 - 589.5831 * diff14 - 1273.2645 * diff23 + 543.132 * diff24 + 54.0957 * diff34 + 66.2114 * contr12 + 114.1404 * contr13 - 1321.0126 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 536.5234 * contr34 + 2470.5963 ;
}

double DistanceEstimator::LM657()
{
	return -1061.5492 * diff12 + 93.0741 * diff13 - 589.5831 * diff14 - 1273.2645 * diff23 + 543.132 * diff24 + 54.0957 * diff34 + 66.2114 * contr12 + 6.757 * contr13 - 1321.0126 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 536.5234 * contr34 + 2463.3934 ;
}

double DistanceEstimator::LM658()
{
	return -2199.9233 * diff12 + 93.0741 * diff13 - 417.2781 * diff14 + 147.5943 * diff23 - 448.5165 * diff24 + 54.0957 * diff34 + 861.9173 * contr12 + 591.1776 * contr13 - 425.7528 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 300.7648 * contr34 + 1759.8835 ;
}

double DistanceEstimator::LM659()
{
	return -2196.686 * diff12 - 991.782 * diff13 - 417.2781 * diff14 + 147.5943 * diff23 + 463.5416 * diff24 + 54.0957 * diff34 - 738.2713 * contr12 + 1084.2826 * contr13 - 4581.2916 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 300.7648 * contr34 + 3485.6037 ;
}

double DistanceEstimator::LM660()
{
	return -189.3751 * diff12 + 605.0369 * diff13 - 436.6308 * diff14 + 184.2195 * diff23 + 327.9714 * diff24 - 933.9688 * diff34 + 21.3593 * contr12 + 20.4086 * contr13 + 2638.8465 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 92.1761 * contr34 + 856.2296 ;
}

double DistanceEstimator::LM661()
{
	return -213.8717 * diff12 + 725.654 * diff13 - 436.6308 * diff14 + 184.2195 * diff23 + 327.9714 * diff24 - 933.9688 * diff34 + 21.3593 * contr12 - 4.3476 * contr13 + 2638.8465 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 92.1761 * contr34 + 818.2387 ;
}

double DistanceEstimator::LM662()
{
	return -213.8717 * diff12 + 714.1666 * diff13 - 436.6308 * diff14 + 184.2195 * diff23 + 327.9714 * diff24 - 933.9688 * diff34 + 21.3593 * contr12 - 4.3476 * contr13 + 2638.8465 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 92.1761 * contr34 + 819.7261 ;
}

double DistanceEstimator::LM663()
{
	return 14.7635 * diff12 - 16.1274 * diff13 - 436.6308 * diff14 + 184.2195 * diff23 + 327.9714 * diff24 - 933.9688 * diff34 - 259.1106 * contr12 + 226.7104 * contr13 + 2638.8465 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 92.1761 * contr34 + 1064.4182 ;
}

double DistanceEstimator::LM664()
{
	return 70.8624 * diff12 + 956.0454 * diff13 - 147.1255 * diff14 + 184.2195 * diff23 + 327.9714 * diff24 - 722.0123 * diff34 + 86.953 * contr12 + 49.3541 * contr13 + 1707.8135 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 92.1761 * contr34 + 895.0581 ;
}

double DistanceEstimator::LM665()
{
	return 70.8624 * diff12 + 956.0454 * diff13 - 147.1255 * diff14 + 184.2195 * diff23 + 327.9714 * diff24 - 722.0123 * diff34 + 86.953 * contr12 + 102.9446 * contr13 + 1642.8396 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 92.1761 * contr34 + 902.4129 ;
}

double DistanceEstimator::LM666()
{
	return 70.8624 * diff12 + 956.0454 * diff13 - 147.1255 * diff14 + 184.2195 * diff23 + 327.9714 * diff24 - 722.0123 * diff34 + 86.953 * contr12 + 102.9446 * contr13 + 1718.3647 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 92.1761 * contr34 + 874.9255 ;
}

double DistanceEstimator::LM667()
{
	return 70.8624 * diff12 + 1076.3912 * diff13 + 209.4169 * diff14 + 184.2195 * diff23 + 327.9714 * diff24 - 722.0123 * diff34 + 262.727 * contr12 + 179.9811 * contr13 + 2262.7979 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 92.1761 * contr34 + 639.1792 ;
}

double DistanceEstimator::LM668()
{
	return 70.8624 * diff12 + 1076.3912 * diff13 + 187.5372 * diff14 + 184.2195 * diff23 + 327.9714 * diff24 - 722.0123 * diff34 + 181.2803 * contr12 + 179.9811 * contr13 + 2262.7979 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 92.1761 * contr34 + 656.1759 ;
}

double DistanceEstimator::LM669()
{
	return -1487.9179 * diff12 + 1713.4796 * diff13 - 1928.5457 * diff14 - 1678.9321 * diff23 + 327.9714 * diff24 - 2576.182 * diff34 + 380.9588 * contr12 + 81.3235 * contr13 + 1442.2133 * contr14 - 14.6062 * contr23 + 4.111 * contr24 + 399.5266 * contr34 + 1930.2214 ;
}

double DistanceEstimator::LM670()
{
	return -4072.0524 * diff12 + 1185.1341 * diff13 - 1427.1712 * diff14 - 286.1005 * diff23 + 327.9714 * diff24 - 1143.1916 * diff34 - 1593.9159 * contr12 + 81.3235 * contr13 + 1442.2133 * contr14 - 14.6062 * contr23 + 1323.292 * contr24 + 92.1761 * contr34 + 1526.4479 ;
}

double DistanceEstimator::LM671()
{
	return -1679.8719 * diff12 + 1185.1341 * diff13 - 1427.1712 * diff14 + 135.7471 * diff23 + 327.9714 * diff24 - 1143.1916 * diff34 - 1214.0131 * contr12 + 81.3235 * contr13 + 1442.2133 * contr14 - 14.6062 * contr23 + 1323.292 * contr24 + 92.1761 * contr34 + 1229.9096 ;
}

double DistanceEstimator::LM672()
{
	return -1477.0839 * diff12 + 1185.1341 * diff13 - 1554.6124 * diff14 - 796.3866 * diff23 + 327.9714 * diff24 - 706.4856 * diff34 + 210.6222 * contr12 + 81.3235 * contr13 + 1442.2133 * contr14 - 14.6062 * contr23 + 1844.0213 * contr24 + 92.1761 * contr34 + 1187.5557 ;
}

double DistanceEstimator::LM673()
{
	return 45.0029 * diff12 + 57.3356 * diff13 - 114.6169 * diff14 - 1120.925 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 152.9256 * contr12 + 2.9274 * contr13 - 26.2073 * contr14 - 6.8201 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 2315.8012 ;
}

double DistanceEstimator::LM674()
{
	return -122.0097 * diff12 - 21.6249 * diff13 - 769.0657 * diff14 - 113.6764 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 1196.227 * contr12 + 50.2642 * contr13 - 26.2073 * contr14 - 6.8201 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 2407.2235 ;
}

double DistanceEstimator::LM675()
{
	return -81.6489 * diff12 - 21.6249 * diff13 - 769.0657 * diff14 - 113.6764 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 1196.227 * contr12 + 50.2642 * contr13 - 26.2073 * contr14 - 6.8201 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 2396.9946 ;
}

double DistanceEstimator::LM676()
{
	return 120.1548 * diff12 - 21.6249 * diff13 - 840.4625 * diff14 - 113.6764 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 1196.227 * contr12 + 50.2642 * contr13 - 26.2073 * contr14 - 6.8201 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 2360.4196 ;
}

double DistanceEstimator::LM677()
{
	return 120.1548 * diff12 + 983.3484 * diff13 - 1974.2547 * diff14 - 113.6764 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 681.5209 * contr12 + 50.2642 * contr13 - 26.2073 * contr14 - 6.8201 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 2077.9064 ;
}

double DistanceEstimator::LM678()
{
	return 120.1548 * diff12 + 1691.32 * diff13 - 1773.3492 * diff14 - 113.6764 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 681.5209 * contr12 + 50.2642 * contr13 - 26.2073 * contr14 - 6.8201 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 1810.2098 ;
}

double DistanceEstimator::LM679()
{
	return 120.1548 * diff12 + 1080.8194 * diff13 - 1504.4497 * diff14 - 113.6764 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 681.5209 * contr12 + 50.2642 * contr13 - 26.2073 * contr14 - 6.8201 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 1952.1618 ;
}

double DistanceEstimator::LM680()
{
	return 120.1548 * diff12 - 2342.8032 * diff13 - 1504.4497 * diff14 - 113.6764 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 681.5209 * contr12 + 50.2642 * contr13 - 26.2073 * contr14 - 6.8201 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 3093.0066 ;
}

double DistanceEstimator::LM681()
{
	return 120.1548 * diff12 - 1690.6846 * diff13 - 1504.4497 * diff14 - 113.6764 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 681.5209 * contr12 + 50.2642 * contr13 - 26.2073 * contr14 - 6.8201 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 2874.625 ;
}

double DistanceEstimator::LM682()
{
	return 1958.3612 * diff12 + 239.2181 * diff13 - 176.4349 * diff14 - 252.7649 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 1066.7406 * contr12 + 61.5472 * contr13 + 61.3782 * contr14 - 6.8201 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 1832.4543 ;
}

double DistanceEstimator::LM683()
{
	return 2501.4832 * diff12 + 239.2181 * diff13 - 176.4349 * diff14 - 252.7649 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 824.6707 * contr12 + 61.5472 * contr13 + 61.3782 * contr14 - 6.8201 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 1824.0422 ;
}

double DistanceEstimator::LM684()
{
	return 403.1115 * diff12 + 239.2181 * diff13 - 176.4349 * diff14 - 645.7282 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 306.9308 * contr12 - 64.8164 * contr13 - 12.5818 * contr14 - 6.8201 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 2125.9455 ;
}

double DistanceEstimator::LM685()
{
	return 403.1115 * diff12 + 239.2181 * diff13 - 176.4349 * diff14 - 719.6768 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 306.9308 * contr12 - 1234.767 * contr13 + 57.5838 * contr14 - 6.8201 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 2298.7494 ;
}

double DistanceEstimator::LM686()
{
	return 403.1115 * diff12 + 239.2181 * diff13 - 176.4349 * diff14 - 692.6135 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 306.9308 * contr12 - 9.4241 * contr13 + 21.9064 * contr14 - 6.8201 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 2109.4329 ;
}

double DistanceEstimator::LM687()
{
	return 403.1115 * diff12 + 239.2181 * diff13 - 176.4349 * diff14 - 641.5878 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 306.9308 * contr12 - 9.4241 * contr13 + 21.9064 * contr14 - 6.8201 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 2092.349 ;
}

double DistanceEstimator::LM688()
{
	return -2387.2479 * diff12 + 6032.2899 * diff13 - 442.4501 * diff14 - 96.6002 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 + 832.4485 * contr12 - 3175.544 * contr13 - 27.9663 * contr14 - 199.8002 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 758.3472 ;
}

double DistanceEstimator::LM689()
{
	return -5790.6545 * diff12 + 4261.4179 * diff13 - 442.4501 * diff14 - 96.6002 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 + 832.4485 * contr12 - 3243.6193 * contr13 - 27.9663 * contr14 - 199.8002 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 1513.6148 ;
}

double DistanceEstimator::LM690()
{
	return -3806.3053 * diff12 + 4261.4179 * diff13 - 442.4501 * diff14 - 96.6002 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 + 832.4485 * contr12 - 4212.9929 * contr13 - 27.9663 * contr14 - 199.8002 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 1419.8488 ;
}

double DistanceEstimator::LM691()
{
	return 221.8138 * diff12 + 3886.0487 * diff13 - 442.4501 * diff14 - 96.6002 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 + 227.5765 * contr12 - 434.0296 * contr13 - 27.9663 * contr14 - 199.8002 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 963.5223 ;
}

double DistanceEstimator::LM692()
{
	return 235.0891 * diff12 - 88.298 * diff13 - 442.4501 * diff14 - 96.6002 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 + 227.5765 * contr12 - 536.8456 * contr13 - 27.9663 * contr14 - 199.8002 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 2187.9874 ;
}

double DistanceEstimator::LM693()
{
	return 209.8606 * diff12 - 88.298 * diff13 - 442.4501 * diff14 - 96.6002 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 + 227.5765 * contr12 - 640.1487 * contr13 - 27.9663 * contr14 - 199.8002 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 2233.1546 ;
}

double DistanceEstimator::LM694()
{
	return 1099.0327 * diff12 - 88.298 * diff13 - 442.4501 * diff14 - 96.6002 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 + 227.5765 * contr12 - 798.7293 * contr13 - 27.9663 * contr14 - 199.8002 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 2076.2503 ;
}

double DistanceEstimator::LM695()
{
	return -548.464 * diff12 + 5290.8522 * diff13 - 2207.2847 * diff14 - 96.6002 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 3550.1163 * contr12 - 85.6035 * contr13 - 27.9663 * contr14 - 2337.3239 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 1281.6203 ;
}

double DistanceEstimator::LM696()
{
	return -548.464 * diff12 + 33089.4237 * diff13 - 1927.7342 * diff14 - 96.6002 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 2957.7276 * contr12 - 85.6035 * contr13 - 27.9663 * contr14 - 3457.9085 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 - 7993.7095 ;
}

double DistanceEstimator::LM697()
{
	return -548.464 * diff12 + 199.6884 * diff13 - 1551.2635 * diff14 - 96.6002 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 3634.4701 * contr12 - 85.6035 * contr13 - 27.9663 * contr14 - 3716.1782 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 2951.2026 ;
}

double DistanceEstimator::LM698()
{
	return -548.464 * diff12 + 199.6884 * diff13 - 1570.0871 * diff14 - 96.6002 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 3634.4701 * contr12 - 85.6035 * contr13 - 27.9663 * contr14 - 3716.1782 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 2955.4416 ;
}

double DistanceEstimator::LM699()
{
	return -548.464 * diff12 + 199.6884 * diff13 - 2006.1858 * diff14 - 96.6002 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 3981.8146 * contr12 - 85.6035 * contr13 - 27.9663 * contr14 - 3716.1782 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 3087.3966 ;
}

double DistanceEstimator::LM700()
{
	return -548.464 * diff12 + 199.6884 * diff13 - 1994.418 * diff14 - 96.6002 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 3981.8146 * contr12 - 85.6035 * contr13 - 27.9663 * contr14 - 3716.1782 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 3085.0862 ;
}

double DistanceEstimator::LM701()
{
	return -548.464 * diff12 + 199.6884 * diff13 - 1927.7342 * diff14 - 96.6002 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 4083.9748 * contr12 - 85.6035 * contr13 - 27.9663 * contr14 - 3716.1782 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 3087.8295 ;
}

double DistanceEstimator::LM702()
{
	return -1170.969 * diff12 + 2784.1661 * diff13 - 1275.4495 * diff14 - 96.6002 * diff23 + 16.4557 * diff24 - 17.6724 * diff34 - 2592.8776 * contr12 - 85.6035 * contr13 - 27.9663 * contr14 - 644.4934 * contr23 + 3.2184 * contr24 + 17.2772 * contr34 + 1858.5021 ;
}

double DistanceEstimator::LM703()
{
	return 135.2371 * diff12 - 34.9778 * diff13 - 19.496 * diff14 - 394.212 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 + 379.9943 * contr12 - 45.0029 * contr13 - 192.7187 * contr23 - 273.1737 * contr24 + 9.527 * contr34 + 2105.8746 ;
}

double DistanceEstimator::LM704()
{
	return 4564.3382 * diff12 - 34.9778 * diff13 - 19.496 * diff14 - 394.212 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 + 941.9908 * contr12 - 45.0029 * contr13 - 192.7187 * contr23 - 273.1737 * contr24 + 9.527 * contr34 + 1635.1874 ;
}

double DistanceEstimator::LM705()
{
	return 3543.626 * diff12 - 34.9778 * diff13 - 19.496 * diff14 - 394.212 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 + 145.6216 * contr12 - 45.0029 * contr13 - 192.7187 * contr23 - 273.1737 * contr24 + 9.527 * contr34 + 1819.4267 ;
}

double DistanceEstimator::LM706()
{
	return 3543.626 * diff12 - 34.9778 * diff13 - 19.496 * diff14 - 394.212 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 + 117.5553 * contr12 - 45.0029 * contr13 - 192.7187 * contr23 - 273.1737 * contr24 + 9.527 * contr34 + 1814.3461 ;
}

double DistanceEstimator::LM707()
{
	return 2562.1973 * diff12 - 34.9778 * diff13 - 19.496 * diff14 - 394.212 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 + 747.0932 * contr12 - 45.0029 * contr13 - 192.7187 * contr23 - 273.1737 * contr24 + 9.527 * contr34 + 1856.3294 ;
}

double DistanceEstimator::LM708()
{
	return 135.2371 * diff12 - 34.9778 * diff13 - 19.496 * diff14 - 538.2958 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 + 119.7232 * contr12 - 45.0029 * contr13 - 192.7187 * contr23 - 273.1737 * contr24 + 9.527 * contr34 + 2143.1829 ;
}

double DistanceEstimator::LM709()
{
	return 927.8976 * diff12 - 34.9778 * diff13 - 3153.4732 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 + 197.0182 * diff34 - 411.3689 * contr12 - 58.8662 * contr13 - 277.7189 * contr23 - 436.9285 * contr24 + 9.527 * contr34 + 2312.3708 ;
}

double DistanceEstimator::LM710()
{
	return 927.8976 * diff12 - 34.9778 * diff13 - 1849.1912 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 + 197.0182 * diff34 - 411.3689 * contr12 - 58.8662 * contr13 - 277.7189 * contr23 - 436.9285 * contr24 + 9.527 * contr34 + 2195.0327 ;
}

double DistanceEstimator::LM711()
{
	return 657.6057 * diff12 + 1941.8331 * diff13 - 284.3684 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 + 73.0852 * diff34 - 657.2574 * contr12 + 844.1494 * contr13 - 639.7746 * contr23 - 436.9285 * contr24 + 9.527 * contr34 + 1421.4211 ;
}

double DistanceEstimator::LM712()
{
	return 657.6057 * diff12 + 1304.718 * diff13 - 41.7979 * diff14 + 1794.3351 * diff23 + 16.4557 * diff24 + 73.0852 * diff34 - 322.5754 * contr12 + 314.163 * contr13 - 639.7746 * contr23 - 436.9285 * contr24 + 9.527 * contr34 + 1211.8769 ;
}

double DistanceEstimator::LM713()
{
	return 657.6057 * diff12 + 1304.718 * diff13 - 41.7979 * diff14 + 559.9522 * diff23 + 16.4557 * diff24 + 73.0852 * diff34 - 191.613 * contr12 + 314.163 * contr13 - 639.7746 * contr23 - 436.9285 * contr24 + 9.527 * contr34 + 1477.7882 ;
}

double DistanceEstimator::LM714()
{
	return 669.5783 * diff12 + 267.0193 * diff13 - 41.7979 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 + 73.0852 * diff34 - 692.8067 * contr12 + 156.3593 * contr13 - 671.2577 * contr23 - 436.9285 * contr24 + 9.527 * contr34 + 1944.7811 ;
}

double DistanceEstimator::LM715()
{
	return 669.5783 * diff12 + 267.0193 * diff13 - 41.7979 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 + 73.0852 * diff34 - 692.8067 * contr12 + 156.3593 * contr13 - 671.2577 * contr23 - 436.9285 * contr24 + 9.527 * contr34 + 1953.4693 ;
}

double DistanceEstimator::LM716()
{
	return 669.5783 * diff12 + 267.0193 * diff13 - 314.1008 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 + 73.0852 * diff34 - 692.8067 * contr12 + 905.0939 * contr13 - 671.2577 * contr23 - 436.9285 * contr24 + 9.527 * contr34 + 1849.9372 ;
}

double DistanceEstimator::LM717()
{
	return 110.1042 * diff12 - 566.9836 * diff13 + 4.6739 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 303.8851 * contr12 - 65.8186 * contr13 + 62.9002 * contr14 - 334.8684 * contr23 - 716.1654 * contr24 + 9.527 * contr34 + 2352.483 ;
}

double DistanceEstimator::LM718()
{
	return 110.1042 * diff12 + 1608.8337 * diff13 + 612.472 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 303.8851 * contr12 - 65.8186 * contr13 + 112.8302 * contr14 - 334.8684 * contr23 - 302.0311 * contr24 + 9.527 * contr34 + 1504.6994 ;
}

double DistanceEstimator::LM719()
{
	return 110.1042 * diff12 + 2161.4227 * diff13 + 1369.5801 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 1871.9352 * contr12 + 1266.4601 * contr13 + 91.9337 * contr14 - 334.8684 * contr23 - 935.0778 * contr24 + 9.527 * contr34 + 1203.4934 ;
}

double DistanceEstimator::LM720()
{
	return 110.1042 * diff12 + 3360.2519 * diff13 + 1213.6636 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 1920.9922 * contr12 + 1486.2958 * contr13 + 91.9337 * contr14 - 334.8684 * contr23 - 935.0778 * contr24 + 9.527 * contr34 + 806.5205 ;
}

double DistanceEstimator::LM721()
{
	return 110.1042 * diff12 + 4038.9438 * diff13 + 751.6038 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 2145.6825 * contr12 + 485.5879 * contr13 - 896.9716 * contr14 - 334.8684 * contr23 - 935.0778 * contr24 + 9.527 * contr34 + 967.284 ;
}

double DistanceEstimator::LM722()
{
	return -475.0498 * diff12 + 1230.433 * diff13 - 19.3632 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 366.4696 * contr12 - 119.6739 * contr13 - 27.6259 * contr23 - 349.9471 * contr24 + 9.527 * contr34 + 1799.2881 ;
}

double DistanceEstimator::LM723()
{
	return -557.631 * diff12 + 2392.4027 * diff13 - 19.3632 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 195.6026 * contr12 - 163.5322 * contr13 - 27.6259 * contr23 - 349.9471 * contr24 + 9.527 * contr34 + 1441.8646 ;
}

double DistanceEstimator::LM724()
{
	return -557.631 * diff12 + 2392.4027 * diff13 - 19.3632 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 366.4696 * contr12 - 183.4678 * contr13 - 27.6259 * contr23 - 349.9471 * contr24 + 9.527 * contr34 + 1454.2087 ;
}

double DistanceEstimator::LM725()
{
	return -557.631 * diff12 + 3102.1676 * diff13 - 19.3632 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 366.4696 * contr12 - 119.6739 * contr13 - 27.6259 * contr23 - 349.9471 * contr24 + 9.527 * contr34 + 1240.3303 ;
}

double DistanceEstimator::LM726()
{
	return -834.3322 * diff12 + 1549.085 * diff13 - 1378.8664 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 7906.0391 * contr12 - 102.2291 * contr13 - 27.6259 * contr23 - 349.9471 * contr24 + 9.527 * contr34 + 2820.3331 ;
}

double DistanceEstimator::LM727()
{
	return -834.3322 * diff12 + 1549.085 * diff13 - 1378.8664 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 7906.0391 * contr12 - 402.445 * contr13 - 27.6259 * contr23 - 349.9471 * contr24 + 9.527 * contr34 + 2887.0745 ;
}

double DistanceEstimator::LM728()
{
	return -834.3322 * diff12 + 1549.085 * diff13 - 1747.2373 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 7621.8054 * contr12 + 1048.1359 * contr13 - 27.6259 * contr23 - 349.9471 * contr24 + 9.527 * contr34 + 2578.2887 ;
}

double DistanceEstimator::LM729()
{
	return 110.1042 * diff12 - 34.9778 * diff13 - 865.9632 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 2478.3416 * contr12 - 1110.6816 * contr13 + 3773.016 * contr23 - 349.9471 * contr24 + 9.527 * contr34 + 2055.2305 ;
}

double DistanceEstimator::LM730()
{
	return -650.113 * diff12 - 34.9778 * diff13 - 1381.0438 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 + 101.6576 * contr12 - 365.5456 * contr13 + 2452.3815 * contr23 - 349.9471 * contr24 + 9.527 * contr34 + 2048.272 ;
}

double DistanceEstimator::LM731()
{
	return -2863.7484 * diff12 - 34.9778 * diff13 - 1276.105 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 121.8542 * contr12 - 365.5456 * contr13 + 2452.3815 * contr23 - 349.9471 * contr24 + 9.527 * contr34 + 2217.5701 ;
}

double DistanceEstimator::LM732()
{
	return 110.1042 * diff12 + 7340.2985 * diff13 + 1817.5794 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 357.2742 * contr12 - 115.125 * contr13 - 32.9722 * contr23 - 1232.2693 * contr24 + 9.527 * contr34 - 328.9785 ;
}

double DistanceEstimator::LM733()
{
	return 110.1042 * diff12 + 7340.2985 * diff13 + 1817.5794 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 3134.9616 * contr12 - 115.125 * contr13 - 32.9722 * contr23 - 1232.2693 * contr24 + 9.527 * contr34 + 23.8804 ;
}

double DistanceEstimator::LM734()
{
	return 110.1042 * diff12 + 8662.1532 * diff13 + 2119.5179 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 1323.2703 * contr12 - 1577.1396 * contr13 - 32.9722 * contr23 - 937.9711 * contr24 + 9.527 * contr34 - 373.7446 ;
}

double DistanceEstimator::LM735()
{
	return 110.1042 * diff12 + 8662.1532 * diff13 + 2119.5179 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 1323.2703 * contr12 - 1333.4705 * contr13 - 32.9722 * contr23 - 937.9711 * contr24 + 9.527 * contr34 - 449.9754 ;
}

double DistanceEstimator::LM736()
{
	return 110.1042 * diff12 + 7095.2215 * diff13 + 1723.1446 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 3176.6035 * contr12 - 115.125 * contr13 + 924.9236 * contr23 - 937.9711 * contr24 + 9.527 * contr34 - 44.3869 ;
}

double DistanceEstimator::LM737()
{
	return 110.1042 * diff12 + 5820.419 * diff13 + 1250.3245 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 357.2742 * contr12 + 1079.6979 * contr13 - 32.9722 * contr23 - 1212.1176 * contr24 + 9.527 * contr34 - 169.3216 ;
}

double DistanceEstimator::LM738()
{
	return 110.1042 * diff12 + 5820.419 * diff13 + 1250.3245 * diff14 - 34.0025 * diff23 + 16.4557 * diff24 - 7.8507 * diff34 - 357.2742 * contr12 + 1519.8959 * contr13 - 32.9722 * contr23 - 1212.1176 * contr24 + 9.527 * contr34 - 255.4207 ;
}

double DistanceEstimator::LM739()
{
	return 558.0476 * diff12 - 7834.8332 * diff13 + 352.8306 * diff14 + 13.027 * diff23 + 16.4557 * diff24 + 1860.3329 * diff34 - 3820.3613 * contr12 - 741.459 * contr13 - 1273.0157 * contr14 - 1703.0771 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 4437.0425 ;
}

double DistanceEstimator::LM740()
{
	return -168.3877 * diff12 + 304.6614 * diff13 + 116.3346 * diff14 + 13.027 * diff23 + 16.4557 * diff24 - 6164.0908 * diff34 + 2492.2274 * contr12 - 741.459 * contr13 + 4.6553 * contr14 + 2523.3144 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 2175.8965 ;
}

double DistanceEstimator::LM741()
{
	return -199.0985 * diff12 + 304.6614 * diff13 + 116.3346 * diff14 + 13.027 * diff23 + 16.4557 * diff24 + 604.2022 * diff34 + 2492.2274 * contr12 - 741.459 * contr13 + 4.6553 * contr14 + 91.4439 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 1558.6284 ;
}

double DistanceEstimator::LM742()
{
	return -893.4712 * diff12 + 304.6614 * diff13 + 2858.8101 * diff14 + 13.027 * diff23 + 16.4557 * diff24 + 156.3997 * diff34 + 4770.7387 * contr12 - 741.459 * contr13 + 914.7762 * contr14 - 874.9452 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 771.0252 ;
}

double DistanceEstimator::LM743()
{
	return 11196.9768 * diff12 + 304.6614 * diff13 + 12704.0837 * diff14 + 13.027 * diff23 + 16.4557 * diff24 + 156.3997 * diff34 + 8989.6758 * contr12 - 741.459 * contr13 + 3128.3641 * contr14 - 874.9452 * contr23 + 3.2184 * contr24 + 9.527 * contr34 - 3363.5812 ;
}

double DistanceEstimator::LM744()
{
	return 8064.4622 * diff12 + 304.6614 * diff13 + 9641.9258 * diff14 + 13.027 * diff23 + 16.4557 * diff24 + 156.3997 * diff34 + 10174.6115 * contr12 - 741.459 * contr13 + 2795.5243 * contr14 - 874.9452 * contr23 + 3.2184 * contr24 + 9.527 * contr34 - 2484.5946 ;
}

double DistanceEstimator::LM745()
{
	return 2733.1891 * diff12 + 216.9465 * diff13 - 100.5815 * diff14 + 3179.8246 * diff23 + 16.4557 * diff24 + 324.5563 * diff34 + 737.8616 * contr12 - 763.3831 * contr13 - 442.2949 * contr14 + 234.8215 * contr23 - 867.5377 * contr24 + 9.527 * contr34 + 1101.0357 ;
}

double DistanceEstimator::LM746()
{
	return 2733.1891 * diff12 + 216.9465 * diff13 - 100.5815 * diff14 + 2403.939 * diff23 + 16.4557 * diff24 + 324.5563 * diff34 + 737.8616 * contr12 - 763.3831 * contr13 - 442.2949 * contr14 + 234.8215 * contr23 - 867.5377 * contr24 + 9.527 * contr34 + 1272.2426 ;
}

double DistanceEstimator::LM747()
{
	return 2733.1891 * diff12 + 216.9465 * diff13 - 100.5815 * diff14 + 2403.939 * diff23 + 16.4557 * diff24 + 324.5563 * diff34 + 737.8616 * contr12 - 763.3831 * contr13 - 442.2949 * contr14 + 234.8215 * contr23 - 867.5377 * contr24 + 9.527 * contr34 + 1267.7169 ;
}

double DistanceEstimator::LM748()
{
	return 2733.1891 * diff12 + 216.9465 * diff13 - 100.5815 * diff14 + 2403.939 * diff23 + 16.4557 * diff24 + 324.5563 * diff34 + 737.8616 * contr12 - 763.3831 * contr13 - 442.2949 * contr14 + 234.8215 * contr23 - 867.5377 * contr24 + 9.527 * contr34 + 1266.8171 ;
}

double DistanceEstimator::LM749()
{
	return 2733.1891 * diff12 + 216.9465 * diff13 - 100.5815 * diff14 + 3179.8246 * diff23 + 16.4557 * diff24 + 324.5563 * diff34 + 737.8616 * contr12 - 763.3831 * contr13 - 442.2949 * contr14 + 234.8215 * contr23 - 867.5377 * contr24 + 9.527 * contr34 + 1118.6826 ;
}

double DistanceEstimator::LM750()
{
	return 968.836 * diff12 + 216.9465 * diff13 - 406.6787 * diff14 + 2370.3321 * diff23 + 16.4557 * diff24 + 324.5563 * diff34 + 1193.037 * contr12 - 763.3831 * contr13 - 442.2949 * contr14 + 234.8215 * contr23 - 1244.2361 * contr24 + 9.527 * contr34 + 1679.3257 ;
}

double DistanceEstimator::LM751()
{
	return 254.1368 * diff12 + 216.9465 * diff13 - 406.6787 * diff14 + 2370.3321 * diff23 + 16.4557 * diff24 + 324.5563 * diff34 + 1193.037 * contr12 - 763.3831 * contr13 - 442.2949 * contr14 + 234.8215 * contr23 - 1296.1945 * contr24 + 9.527 * contr34 + 1788.4622 ;
}

double DistanceEstimator::LM752()
{
	return 363.4477 * diff12 + 216.9465 * diff13 - 223.1223 * diff14 + 2370.3321 * diff23 + 16.4557 * diff24 + 324.5563 * diff34 + 1193.037 * contr12 - 763.3831 * contr13 - 442.2949 * contr14 + 234.8215 * contr23 - 1296.1945 * contr24 + 9.527 * contr34 + 1736.4422 ;
}

double DistanceEstimator::LM753()
{
	return 1025.8194 * diff12 + 216.9465 * diff13 - 614.3875 * diff14 + 2370.3321 * diff23 + 16.4557 * diff24 + 324.5563 * diff34 + 1193.037 * contr12 - 763.3831 * contr13 - 442.2949 * contr14 + 234.8215 * contr23 - 867.5377 * contr24 + 9.527 * contr34 + 1592.2577 ;
}

double DistanceEstimator::LM754()
{
	return -8498.3745 * diff12 + 10395.235 * diff13 - 100.5815 * diff14 + 2519.326 * diff23 + 16.4557 * diff24 + 324.5563 * diff34 + 2465.2136 * contr12 - 763.3831 * contr13 - 442.2949 * contr14 + 234.8215 * contr23 - 1520.6048 * contr24 + 9.527 * contr34 - 643.1489 ;
}

double DistanceEstimator::LM755()
{
	return 15.1807 * diff12 + 1597.6855 * diff13 - 100.5815 * diff14 + 13.027 * diff23 + 16.4557 * diff24 + 1375.5376 * diff34 + 5573.5458 * contr12 - 763.3831 * contr13 - 442.2949 * contr14 + 234.8215 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 875.3639 ;
}

double DistanceEstimator::LM756()
{
	return 15.1807 * diff12 + 1597.6855 * diff13 - 100.5815 * diff14 + 13.027 * diff23 + 16.4557 * diff24 + 1375.5376 * diff34 + 5795.2248 * contr12 - 763.3831 * contr13 - 442.2949 * contr14 + 234.8215 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 874.2298 ;
}

double DistanceEstimator::LM757()
{
	return 15.1807 * diff12 + 413.9904 * diff13 - 100.5815 * diff14 + 13.027 * diff23 + 16.4557 * diff24 + 903.1889 * diff34 + 2176.3834 * contr12 - 763.3831 * contr13 - 442.2949 * contr14 + 234.8215 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 1695.3291 ;
}

double DistanceEstimator::LM758()
{
	return 15.1807 * diff12 + 216.9465 * diff13 - 474.3725 * diff14 + 13.027 * diff23 + 16.4557 * diff24 + 378.6483 * diff34 + 2051.4154 * contr12 - 972.8122 * contr13 - 442.2949 * contr14 + 628.928 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 1927.8228 ;
}

double DistanceEstimator::LM759()
{
	return 351.1682 * diff12 + 216.9465 * diff13 - 371.5799 * diff14 + 13.027 * diff23 + 16.4557 * diff24 + 378.6483 * diff34 + 1235.7997 * contr12 - 972.8122 * contr13 - 442.2949 * contr14 + 901.4626 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 1908.3161 ;
}

double DistanceEstimator::LM760()
{
	return 15.1807 * diff12 + 216.9465 * diff13 - 371.5799 * diff14 + 13.027 * diff23 + 16.4557 * diff24 + 378.6483 * diff34 + 1235.7997 * contr12 - 972.8122 * contr13 - 442.2949 * contr14 + 797.2582 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 1978.5956 ;
}

double DistanceEstimator::LM761()
{
	return -590.0156 * diff12 - 8667.0599 * diff13 - 290.1946 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 1587.8425 * contr12 - 299.5189 * contr13 + 181.7283 * contr14 + 1380.9278 * contr23 - 1651.4108 * contr24 + 2456.9137 * contr34 + 4277.4511 ;
}

double DistanceEstimator::LM762()
{
	return -424.2586 * diff12 - 18027.0078 * diff13 - 290.1946 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 1105.8487 * contr12 - 299.5189 * contr13 + 495.1549 * contr14 + 7618.0423 * contr23 - 1520.7822 * contr24 + 2456.9137 * contr34 + 6566.8909 ;
}

double DistanceEstimator::LM763()
{
	return -424.2586 * diff12 - 19130.4659 * diff13 - 290.1946 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 1105.8487 * contr12 - 299.5189 * contr13 + 495.1549 * contr14 + 7572.1921 * contr23 - 1520.7822 * contr24 + 2456.9137 * contr34 + 6931.861 ;
}

double DistanceEstimator::LM764()
{
	return -424.2586 * diff12 - 13231.6553 * diff13 - 290.1946 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 1105.8487 * contr12 - 299.5189 * contr13 + 438.6422 * contr14 + 3643.3539 * contr23 - 1520.7822 * contr24 + 2456.9137 * contr34 + 5440.2938 ;
}

double DistanceEstimator::LM765()
{
	return 2485.4356 * diff12 - 2841.4294 * diff13 + 628.5825 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 381.7857 * contr12 - 134.7316 * contr13 + 1945.7529 * contr14 + 1434.3156 * contr23 + 3.2184 * contr24 + 5544.882 * contr34 + 754.4685 ;
}

double DistanceEstimator::LM766()
{
	return 2485.4356 * diff12 - 2841.4294 * diff13 + 628.5825 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 381.7857 * contr12 - 134.7316 * contr13 + 1945.7529 * contr14 + 1434.3156 * contr23 + 3.2184 * contr24 + 5544.882 * contr34 + 765.2993 ;
}

double DistanceEstimator::LM767()
{
	return 2485.4356 * diff12 - 2841.4294 * diff13 + 628.5825 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 381.7857 * contr12 - 134.7316 * contr13 + 1945.7529 * contr14 + 1434.3156 * contr23 + 3.2184 * contr24 + 5544.882 * contr34 + 767.9622 ;
}

double DistanceEstimator::LM768()
{
	return 2485.4356 * diff12 - 2841.4294 * diff13 + 628.5825 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 381.7857 * contr12 - 134.7316 * contr13 + 642.8706 * contr14 + 1434.3156 * contr23 + 3.2184 * contr24 + 4807.06 * contr34 + 1063.8329 ;
}

double DistanceEstimator::LM769()
{
	return 2131.2066 * diff12 - 2611.5927 * diff13 + 1134.7552 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 1211.3516 * contr12 - 134.7316 * contr13 + 138.9443 * contr14 + 1100.2992 * contr23 + 3.2184 * contr24 + 1473.3075 * contr34 + 1927.9325 ;
}

double DistanceEstimator::LM770()
{
	return 2131.2066 * diff12 - 2611.5927 * diff13 + 1546.5774 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 628.6088 * contr12 - 134.7316 * contr13 - 9.8637 * contr14 + 1100.2992 * contr23 + 3.2184 * contr24 + 1473.3075 * contr34 + 1802.8676 ;
}

double DistanceEstimator::LM771()
{
	return 2131.2066 * diff12 - 2611.5927 * diff13 + 1139.0138 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 71.3964 * contr12 - 134.7316 * contr13 - 9.8637 * contr14 + 1100.2992 * contr23 - 1069.063 * contr24 + 1473.3075 * contr34 + 2047.0189 ;
}

double DistanceEstimator::LM772()
{
	return 2131.2066 * diff12 - 2611.5927 * diff13 + 1139.0138 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 71.3964 * contr12 - 134.7316 * contr13 - 9.8637 * contr14 + 1100.2992 * contr23 - 800.9927 * contr24 + 1473.3075 * contr34 + 1984.1302 ;
}

double DistanceEstimator::LM773()
{
	return 2050.4587 * diff12 - 2611.5927 * diff13 + 1139.0138 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 71.3964 * contr12 - 134.7316 * contr13 - 9.8637 * contr14 + 1100.2992 * contr23 - 800.9927 * contr24 + 1473.3075 * contr34 + 1993.8552 ;
}

double DistanceEstimator::LM774()
{
	return 2131.2066 * diff12 - 2611.5927 * diff13 + 1139.0138 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 144.2466 * contr12 - 134.7316 * contr13 - 9.8637 * contr14 + 848.252 * contr23 + 3.2184 * contr24 + 1473.3075 * contr34 + 1853.58 ;
}

double DistanceEstimator::LM775()
{
	return 2131.2066 * diff12 - 2611.5927 * diff13 + 1139.0138 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 71.3964 * contr12 - 134.7316 * contr13 - 9.8637 * contr14 + 848.252 * contr23 + 3.2184 * contr24 + 1473.3075 * contr34 + 1839.46 ;
}

double DistanceEstimator::LM776()
{
	return 2131.2066 * diff12 - 2611.5927 * diff13 + 1139.0138 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 71.3964 * contr12 - 134.7316 * contr13 - 9.8637 * contr14 + 519.9396 * contr23 + 3.2184 * contr24 + 1473.3075 * contr34 + 1867.9573 ;
}

double DistanceEstimator::LM777()
{
	return 30194.6845 * diff12 + 4946.3453 * diff13 + 740.9311 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 1378.5278 * contr12 - 134.7316 * contr13 - 154.5382 * contr14 + 6657.1765 * contr23 + 3.2184 * contr24 + 1473.3075 * contr34 - 4797.6939 ;
}

double DistanceEstimator::LM778()
{
	return 9672.834 * diff12 - 3539.4026 * diff13 + 740.9311 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 115.2691 * contr12 - 134.7316 * contr13 - 154.5382 * contr14 + 3765.2715 * contr23 + 3.2184 * contr24 + 1473.3075 * contr34 + 1100.4001 ;
}

double DistanceEstimator::LM779()
{
	return 9783.5796 * diff12 - 3539.4026 * diff13 + 740.9311 * diff14 - 387.5569 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 115.2691 * contr12 - 134.7316 * contr13 - 154.5382 * contr14 + 3765.2715 * contr23 + 3.2184 * contr24 + 1473.3075 * contr34 + 1083.6596 ;
}

double DistanceEstimator::LM780()
{
	return 1070.2468 * diff12 + 2198.1507 * diff13 - 1069.7522 * diff14 + 1416.1274 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 1940.3339 * contr12 - 20.95 * contr13 - 4184.4546 * contr23 - 3367.4995 * contr24 - 1324.3659 * contr34 + 2714.3946 ;
}

double DistanceEstimator::LM781()
{
	return 15.1807 * diff12 - 120.7997 * diff13 - 2215.2456 * diff14 + 2829.7537 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 3701.4902 * contr12 - 20.95 * contr13 + 106.9297 * contr23 + 3.2184 * contr24 - 1324.3659 * contr34 + 2276.7443 ;
}

double DistanceEstimator::LM782()
{
	return 15.1807 * diff12 - 662.1026 * diff13 - 2215.2456 * diff14 + 2829.7537 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 3701.4902 * contr12 - 20.95 * contr13 - 240.8873 * contr23 + 3.2184 * contr24 - 1324.3659 * contr34 + 2484.4052 ;
}

double DistanceEstimator::LM783()
{
	return 15.1807 * diff12 - 770.8715 * diff13 - 2215.2456 * diff14 + 2829.7537 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 3701.4902 * contr12 - 20.95 * contr13 - 240.8873 * contr23 + 3.2184 * contr24 - 1324.3659 * contr34 + 2515.1006 ;
}

double DistanceEstimator::LM784()
{
	return 15.1807 * diff12 + 2964.4631 * diff13 - 3160.1528 * diff14 + 1587.0388 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 5998.9942 * contr12 - 20.95 * contr13 - 1189.538 * contr23 + 3.2184 * contr24 - 1324.3659 * contr34 + 2129.9644 ;
}

double DistanceEstimator::LM785()
{
	return -1725.0674 * diff12 - 809.4502 * diff13 - 5377.5261 * diff14 + 1587.0388 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 8096.772 * contr12 - 20.95 * contr13 - 2458.2928 * contr14 - 946.5586 * contr23 + 3.2184 * contr24 - 1324.3659 * contr34 + 4325.8179 ;
}

double DistanceEstimator::LM786()
{
	return -2484.2975 * diff12 - 809.4502 * diff13 - 7353.4453 * diff14 + 1587.0388 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 10211.2575 * contr12 + 3499.8269 * contr13 - 1205.0455 * contr14 - 946.5586 * contr23 + 3.2184 * contr24 - 1324.3659 * contr34 + 4194.8624 ;
}

double DistanceEstimator::LM787()
{
	return 15.1807 * diff12 - 471.5972 * diff13 - 383.1696 * diff14 - 592.3768 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 79.1531 * contr12 - 78.0331 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 2609.9481 ;
}

double DistanceEstimator::LM788()
{
	return 15.1807 * diff12 + 284.8166 * diff13 - 383.1696 * diff14 - 592.3768 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 79.1531 * contr12 + 41.1326 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 2360.9264 ;
}

double DistanceEstimator::LM789()
{
	return 15.1807 * diff12 + 4748.1477 * diff13 - 383.1696 * diff14 - 592.3768 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 79.1531 * contr12 + 280.9473 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 926.4068 ;
}

double DistanceEstimator::LM790()
{
	return 15.1807 * diff12 + 2697.9444 * diff13 + 120.8433 * diff14 - 592.3768 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 79.1531 * contr12 - 251.1273 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 1560.382 ;
}

double DistanceEstimator::LM791()
{
	return 15.1807 * diff12 + 2697.9444 * diff13 - 383.1696 * diff14 - 592.3768 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 79.1531 * contr12 - 325.75 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 1661.4431 ;
}

double DistanceEstimator::LM792()
{
	return 15.1807 * diff12 + 2697.9444 * diff13 - 383.1696 * diff14 - 592.3768 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 79.1531 * contr12 + 313.5325 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 1572.0907 ;
}

double DistanceEstimator::LM793()
{
	return 15.1807 * diff12 - 471.5972 * diff13 - 383.1696 * diff14 - 2624.0812 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 79.1531 * contr12 - 1074.9467 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 3252.9558 ;
}

double DistanceEstimator::LM794()
{
	return 15.1807 * diff12 + 1668.4245 * diff13 - 383.1696 * diff14 - 2473.5846 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 79.1531 * contr12 - 1857.9083 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 2725.5693 ;
}

double DistanceEstimator::LM795()
{
	return 15.1807 * diff12 + 7516.1874 * diff13 - 383.1696 * diff14 - 2473.5846 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 79.1531 * contr12 - 2037.4779 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 955.877 ;
}

double DistanceEstimator::LM796()
{
	return 15.1807 * diff12 + 7516.1874 * diff13 - 383.1696 * diff14 - 2473.5846 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 79.1531 * contr12 - 2037.4779 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 957.2859 ;
}

double DistanceEstimator::LM797()
{
	return 15.1807 * diff12 - 433.3654 * diff13 - 383.1696 * diff14 - 874.6625 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 79.1531 * contr12 - 20.95 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 2676.784 ;
}

double DistanceEstimator::LM798()
{
	return 15.1807 * diff12 - 433.3654 * diff13 - 383.1696 * diff14 - 745.0572 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 79.1531 * contr12 - 20.95 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 2634.2533 ;
}

double DistanceEstimator::LM799()
{
	return 15.1807 * diff12 - 598.2077 * diff13 - 383.1696 * diff14 + 3296.4106 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 2316.8093 * contr12 - 3852.3236 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 2259.8839 ;
}

double DistanceEstimator::LM800()
{
	return 15.1807 * diff12 - 598.2077 * diff13 - 383.1696 * diff14 - 1800.9854 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 1894.8332 * contr12 - 324.1445 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 2725.6864 ;
}

double DistanceEstimator::LM801()
{
	return 15.1807 * diff12 - 598.2077 * diff13 - 383.1696 * diff14 - 2391.7092 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 1894.8332 * contr12 - 829.9074 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 2932.4631 ;
}

double DistanceEstimator::LM802()
{
	return 15.1807 * diff12 - 598.2077 * diff13 - 383.1696 * diff14 - 619.5376 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 1415.8691 * contr12 - 20.95 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 2495.439 ;
}

double DistanceEstimator::LM803()
{
	return 15.1807 * diff12 - 598.2077 * diff13 - 383.1696 * diff14 - 619.5376 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 79.1531 * contr12 - 20.95 * contr13 + 115.3874 * contr14 - 50.7432 * contr23 - 76.3587 * contr24 - 776.5679 * contr34 + 2670.5644 ;
}

double DistanceEstimator::LM804()
{
	return 15.1807 * diff12 + 998.8301 * diff13 - 659.1415 * diff14 + 537.2423 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 79.1531 * contr12 - 882.9583 * contr13 - 816.9906 * contr14 - 50.7432 * contr23 - 163.747 * contr24 - 3558.6132 * contr34 + 3018.483 ;
}

double DistanceEstimator::LM805()
{
	return 15.1807 * diff12 + 998.8301 * diff13 - 650.5131 * diff14 + 537.2423 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 79.1531 * contr12 - 882.9583 * contr13 - 816.9906 * contr14 - 50.7432 * contr23 - 163.747 * contr24 - 3558.6132 * contr34 + 3010.5565 ;
}

double DistanceEstimator::LM806()
{
	return 15.1807 * diff12 + 781.4686 * diff13 - 434.8033 * diff14 + 2920.6341 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 1836.8326 * contr12 - 641.596 * contr13 - 520.4449 * contr14 - 50.7432 * contr23 - 163.747 * contr24 - 3558.6132 * contr34 + 2212.1409 ;
}

double DistanceEstimator::LM807()
{
	return 15.1807 * diff12 + 781.4686 * diff13 - 434.8033 * diff14 + 5113.5475 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 4188.978 * contr12 - 641.596 * contr13 - 520.4449 * contr14 - 50.7432 * contr23 - 163.747 * contr24 - 3558.6132 * contr34 + 1430.2887 ;
}

double DistanceEstimator::LM808()
{
	return 15.1807 * diff12 + 781.4686 * diff13 - 434.8033 * diff14 + 4360.9218 * diff23 + 16.4557 * diff24 + 59.005 * diff34 + 3347.1334 * contr12 - 641.596 * contr13 - 520.4449 * contr14 - 50.7432 * contr23 - 163.747 * contr24 - 3558.6132 * contr34 + 1690.34 ;
}

double DistanceEstimator::LM809()
{
	return 15.1807 * diff12 + 689.1754 * diff13 - 434.8033 * diff14 - 499.6369 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 9958.4327 * contr12 + 1816.3846 * contr13 + 242.1012 * contr14 - 820.9806 * contr23 - 163.747 * contr24 - 2715.857 * contr34 + 3619.1745 ;
}

double DistanceEstimator::LM810()
{
	return 15.1807 * diff12 + 689.1754 * diff13 - 434.8033 * diff14 - 499.6369 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 9958.4327 * contr12 + 3417.791 * contr13 + 242.1012 * contr14 - 820.9806 * contr23 - 163.747 * contr24 - 2715.857 * contr34 + 3214.4087 ;
}

double DistanceEstimator::LM811()
{
	return 15.1807 * diff12 + 689.1754 * diff13 - 434.8033 * diff14 - 499.6369 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 9958.4327 * contr12 + 3506.758 * contr13 + 242.1012 * contr14 - 820.9806 * contr23 - 163.747 * contr24 - 2715.857 * contr34 + 3200.3235 ;
}

double DistanceEstimator::LM812()
{
	return 15.1807 * diff12 + 689.1754 * diff13 - 434.8033 * diff14 - 1696.5972 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 4039.1046 * contr12 - 20.95 * contr13 + 242.1012 * contr14 - 1145.6045 * contr23 - 163.747 * contr24 - 2715.857 * contr34 + 3519.5614 ;
}

double DistanceEstimator::LM813()
{
	return 15.1807 * diff12 + 689.1754 * diff13 - 434.8033 * diff14 - 499.6369 * diff23 + 16.4557 * diff24 + 59.005 * diff34 - 7173.1487 * contr12 - 20.95 * contr13 + 242.1012 * contr14 - 73.1818 * contr23 - 163.747 * contr24 - 2715.857 * contr34 + 3522.4747 ;
}

double DistanceEstimator::LM814()
{
	return 15.1807 * diff12 + 860.0154 * diff13 - 434.8033 * diff14 - 499.6369 * diff23 + 16.4557 * diff24 - 692.0269 * diff34 - 1544.8716 * contr12 - 20.95 * contr13 + 242.1012 * contr14 + 465.36 * contr23 - 163.747 * contr24 - 2715.857 * contr34 + 2950.6787 ;
}

double DistanceEstimator::LM815()
{
	return 15.1807 * diff12 + 860.0154 * diff13 - 434.8033 * diff14 - 499.6369 * diff23 + 16.4557 * diff24 - 412.5732 * diff34 - 1544.8716 * contr12 - 20.95 * contr13 + 242.1012 * contr14 + 931.1587 * contr23 - 163.747 * contr24 - 2715.857 * contr34 + 2817.8433 ;
}

double DistanceEstimator::LM816()
{
	return -127.3769 * diff12 - 1044.1385 * diff13 - 499.6143 * diff14 - 338.3442 * diff23 + 16.4557 * diff24 + 152.3249 * diff34 - 532.2523 * contr12 + 39.3231 * contr13 + 62.6097 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 2637.3559 ;
}

double DistanceEstimator::LM817()
{
	return -127.3769 * diff12 - 1044.1385 * diff13 - 261.3899 * diff14 - 723.284 * diff23 + 16.4557 * diff24 + 152.3249 * diff34 - 448.4965 * contr12 + 162.8896 * contr13 + 62.6097 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 2591.9215 ;
}

double DistanceEstimator::LM818()
{
	return -127.3769 * diff12 - 1044.1385 * diff13 - 261.3899 * diff14 - 1030.1155 * diff23 + 16.4557 * diff24 + 152.3249 * diff34 + 671.2849 * contr12 + 162.8896 * contr13 + 62.6097 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 2504.1753 ;
}

double DistanceEstimator::LM819()
{
	return -127.3769 * diff12 - 1044.1385 * diff13 - 261.3899 * diff14 - 901.1485 * diff23 + 16.4557 * diff24 + 152.3249 * diff34 + 126.2778 * contr12 + 162.8896 * contr13 + 62.6097 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 2547.2619 ;
}

double DistanceEstimator::LM820()
{
	return -127.3769 * diff12 - 1044.1385 * diff13 - 261.3899 * diff14 - 313.5547 * diff23 + 16.4557 * diff24 + 152.3249 * diff34 - 199.8339 * contr12 + 130.1066 * contr13 + 62.6097 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 2517.9283 ;
}

double DistanceEstimator::LM821()
{
	return -127.3769 * diff12 - 2157.1664 * diff13 - 82.7216 * diff14 - 63.3928 * diff23 + 16.4557 * diff24 + 152.3249 * diff34 - 352.4556 * contr12 - 95.245 * contr13 + 62.6097 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 2862.4681 ;
}

double DistanceEstimator::LM822()
{
	return -127.3769 * diff12 - 2157.1664 * diff13 - 82.7216 * diff14 - 63.3928 * diff23 + 16.4557 * diff24 + 152.3249 * diff34 - 352.4556 * contr12 - 109.0598 * contr13 + 62.6097 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 2857.1672 ;
}

double DistanceEstimator::LM823()
{
	return -127.3769 * diff12 - 5533.2941 * diff13 - 82.7216 * diff14 - 63.3928 * diff23 + 16.4557 * diff24 + 152.3249 * diff34 - 205.968 * contr12 - 700.9817 * contr13 + 62.6097 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 4043.5914 ;
}

double DistanceEstimator::LM824()
{
	return -127.3769 * diff12 - 2530.3757 * diff13 - 82.7216 * diff14 - 63.3928 * diff23 + 16.4557 * diff24 + 152.3249 * diff34 - 881.6648 * contr12 + 19.8784 * contr13 + 62.6097 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 3037.773 ;
}

double DistanceEstimator::LM825()
{
	return -127.3769 * diff12 - 2530.3757 * diff13 - 82.7216 * diff14 - 63.3928 * diff23 + 16.4557 * diff24 + 152.3249 * diff34 - 1668.2869 * contr12 + 19.8784 * contr13 + 62.6097 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 3143.6898 ;
}

double DistanceEstimator::LM826()
{
	return -264.136 * diff12 - 2530.3757 * diff13 - 82.7216 * diff14 - 63.3928 * diff23 + 16.4557 * diff24 + 152.3249 * diff34 - 2365.6691 * contr12 + 19.8784 * contr13 + 62.6097 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 3279.7984 ;
}

double DistanceEstimator::LM827()
{
	return -264.136 * diff12 - 2530.3757 * diff13 - 82.7216 * diff14 - 63.3928 * diff23 + 16.4557 * diff24 + 152.3249 * diff34 - 2365.6691 * contr12 + 19.8784 * contr13 + 62.6097 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 3279.4538 ;
}

double DistanceEstimator::LM828()
{
	return -127.3769 * diff12 - 2530.3757 * diff13 - 82.7216 * diff14 - 63.3928 * diff23 + 16.4557 * diff24 + 152.3249 * diff34 - 2365.6691 * contr12 + 19.8784 * contr13 + 62.6097 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 3258.2079 ;
}

double DistanceEstimator::LM829()
{
	return -127.3769 * diff12 - 2530.3757 * diff13 - 82.7216 * diff14 - 63.3928 * diff23 + 16.4557 * diff24 + 152.3249 * diff34 - 2598.9924 * contr12 + 19.8784 * contr13 + 62.6097 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 3291.5256 ;
}

double DistanceEstimator::LM830()
{
	return 255.5446 * diff12 + 994.378 * diff13 - 123.2832 * diff14 + 768.0023 * diff23 + 16.4557 * diff24 + 248.9598 * diff34 + 521.6797 * contr12 - 22.501 * contr13 + 856.1088 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 1352.068 ;
}

double DistanceEstimator::LM831()
{
	return 255.5446 * diff12 + 1032.8671 * diff13 - 123.2832 * diff14 - 2373.072 * diff23 + 16.4557 * diff24 + 248.9598 * diff34 + 521.6797 * contr12 - 22.501 * contr13 + 856.1088 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 1842.178 ;
}

double DistanceEstimator::LM832()
{
	return 724.4719 * diff12 + 1525.6421 * diff13 - 625.26 * diff14 + 768.0023 * diff23 + 16.4557 * diff24 + 248.9598 * diff34 + 1114.2885 * contr12 - 22.501 * contr13 + 856.1088 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 1133.2698 ;
}

double DistanceEstimator::LM833()
{
	return 724.4719 * diff12 + 1787.8675 * diff13 - 625.26 * diff14 + 768.0023 * diff23 + 16.4557 * diff24 + 248.9598 * diff34 + 1114.2885 * contr12 - 22.501 * contr13 + 856.1088 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 1045.1657 ;
}

double DistanceEstimator::LM834()
{
	return 724.4719 * diff12 + 1807.3995 * diff13 - 625.26 * diff14 + 768.0023 * diff23 + 16.4557 * diff24 + 248.9598 * diff34 + 1114.2885 * contr12 - 22.501 * contr13 + 856.1088 * contr23 + 3.2184 * contr24 + 9.527 * contr34 + 1050.3379 ;
}

double DistanceEstimator::LM835()
{
	return -3576.1211 * diff12 - 352.7407 * diff13 + 2069.4832 * diff14 + 1085.013 * diff23 + 16.4557 * diff24 + 248.9598 * diff34 - 1099.7577 * contr12 - 1370.2541 * contr13 + 878.6382 * contr23 - 1778.4239 * contr24 + 9.527 * contr34 + 2842.1568 ;
}

double DistanceEstimator::LM836()
{
	return -3576.1211 * diff12 - 352.7407 * diff13 + 3660.8285 * diff14 - 2640.8997 * diff23 + 16.4557 * diff24 + 248.9598 * diff34 - 1099.7577 * contr12 - 22.501 * contr13 + 878.6382 * contr23 - 1778.4239 * contr24 + 9.527 * contr34 + 2875.4058 ;
}

double DistanceEstimator::LM837()
{
	return -3576.1211 * diff12 - 352.7407 * diff13 + 3660.8285 * diff14 - 2640.8997 * diff23 + 16.4557 * diff24 + 248.9598 * diff34 + 95.5479 * contr12 - 22.501 * contr13 + 878.6382 * contr23 - 1778.4239 * contr24 + 9.527 * contr34 + 2747.9038 ;
}

double DistanceEstimator::LM838()
{
	return -3576.1211 * diff12 - 352.7407 * diff13 + 3905.6508 * diff14 - 2640.8997 * diff23 + 16.4557 * diff24 + 248.9598 * diff34 - 972.0398 * contr12 - 22.501 * contr13 + 878.6382 * contr23 - 1778.4239 * contr24 + 9.527 * contr34 + 2846.8503 ;
}

double DistanceEstimator::LM839()
{
	return -3576.1211 * diff12 - 352.7407 * diff13 + 3905.6508 * diff14 - 2640.8997 * diff23 + 16.4557 * diff24 + 248.9598 * diff34 - 989.4558 * contr12 - 22.501 * contr13 + 878.6382 * contr23 - 1778.4239 * contr24 + 9.527 * contr34 + 2851.544 ;
}

double DistanceEstimator::LM840()
{
	return 4.7242 * diff12 - 1905.8618 * diff13 - 1897.986 * diff14 + 1307.6956 * diff23 + 2.6997 * diff24 + 234.1094 * diff34 - 980.5741 * contr12 + 361.3231 * contr13 + 2.2353 * contr14 - 6.7921 * contr23 + 336.919 * contr24 - 7.2842 * contr34 + 2490.4927 ;
}

double DistanceEstimator::LM841()
{
	return 4.7242 * diff12 - 1905.8618 * diff13 - 1897.986 * diff14 + 1307.6956 * diff23 + 2.6997 * diff24 + 234.1094 * diff34 - 349.7323 * contr12 + 361.3231 * contr13 + 2.2353 * contr14 - 6.7921 * contr23 + 336.919 * contr24 - 7.2842 * contr34 + 2401.2869 ;
}

double DistanceEstimator::LM842()
{
	return 4.7242 * diff12 - 1045.6954 * diff13 - 248.7045 * diff14 + 1410.3382 * diff23 + 2.6997 * diff24 + 234.1094 * diff34 - 182.3295 * contr12 + 92.2132 * contr13 + 2.2353 * contr14 - 6.7921 * contr23 + 116.6562 * contr24 - 7.2842 * contr34 + 1824.9593 ;
}

double DistanceEstimator::LM843()
{
	return 4.7242 * diff12 - 1045.6954 * diff13 - 248.7045 * diff14 + 1572.984 * diff23 + 2.6997 * diff24 + 234.1094 * diff34 - 1010.0925 * contr12 + 165.8365 * contr13 + 2.2353 * contr14 - 6.7921 * contr23 + 116.6562 * contr24 - 7.2842 * contr34 + 1896.5886 ;
}

double DistanceEstimator::LM844()
{
	return 4.7242 * diff12 - 1045.6954 * diff13 - 248.7045 * diff14 + 1572.984 * diff23 + 2.6997 * diff24 + 234.1094 * diff34 - 640.8426 * contr12 + 269.9247 * contr13 + 2.2353 * contr14 - 6.7921 * contr23 + 116.6562 * contr24 - 7.2842 * contr34 + 1848.0524 ;
}

double DistanceEstimator::LM845()
{
	return -5807.4787 * diff12 - 1045.6954 * diff13 - 248.7045 * diff14 + 11980.9574 * diff23 + 2.6997 * diff24 + 234.1094 * diff34 - 308.7594 * contr12 + 92.2132 * contr13 + 2.2353 * contr14 - 6.7921 * contr23 + 1518.0296 * contr24 - 7.2842 * contr34 - 49.5637 ;
}

double DistanceEstimator::LM846()
{
	return -5342.5024 * diff12 + 848.1484 * diff13 - 248.7045 * diff14 + 7103.6397 * diff23 + 2.6997 * diff24 + 234.1094 * diff34 - 308.7594 * contr12 + 92.2132 * contr13 + 2.2353 * contr14 - 6.7921 * contr23 + 1405.9197 * contr24 - 7.2842 * contr34 + 362.9744 ;
}

double DistanceEstimator::LM847()
{
	return 4.7242 * diff12 - 1058.9504 * diff13 - 4.6684 * diff14 + 388.9589 * diff23 + 2.6997 * diff24 + 438.6496 * diff34 + 238.9091 * contr12 - 920.4657 * contr13 + 2.2353 * contr14 + 196.9057 * contr23 + 602.5825 * contr24 - 7.2842 * contr34 + 1763.6334 ;
}

double DistanceEstimator::LM848()
{
	return 619.1881 * diff12 - 1058.9504 * diff13 - 1280.4749 * diff14 + 3208.7725 * diff23 + 2.6997 * diff24 + 438.6496 * diff34 + 1651.997 * contr12 - 4818.915 * contr13 + 2.2353 * contr14 + 196.9057 * contr23 + 171.469 * contr24 - 7.2842 * contr34 + 1999.5454 ;
}

double DistanceEstimator::LM849()
{
	return -203.2949 * diff12 - 1058.9504 * diff13 - 1467.1182 * diff14 + 2434.1222 * diff23 + 2.6997 * diff24 + 438.6496 * diff34 + 1099.7271 * contr12 - 2452.6345 * contr13 + 2.2353 * contr14 + 196.9057 * contr23 + 171.469 * contr24 - 7.2842 * contr34 + 2029.8084 ;
}

double DistanceEstimator::LM850()
{
	return -203.2949 * diff12 - 1058.9504 * diff13 - 1467.1182 * diff14 + 2434.1222 * diff23 + 2.6997 * diff24 + 438.6496 * diff34 + 1099.7271 * contr12 - 2533.9899 * contr13 + 2.2353 * contr14 + 196.9057 * contr23 + 171.469 * contr24 - 7.2842 * contr34 + 2048.5257 ;
}

double DistanceEstimator::LM851()
{
	return -791.3408 * diff12 - 1058.9504 * diff13 - 1619.0706 * diff14 + 2434.1222 * diff23 + 2.6997 * diff24 + 438.6496 * diff34 + 1896.6272 * contr12 - 2087.202 * contr13 + 2.2353 * contr14 + 196.9057 * contr23 + 171.469 * contr24 - 7.2842 * contr34 + 2057.0348 ;
}

double DistanceEstimator::LM852()
{
	return -352.8373 * diff12 - 1058.9504 * diff13 - 1619.0706 * diff14 + 2434.1222 * diff23 + 2.6997 * diff24 + 438.6496 * diff34 + 1960.9991 * contr12 - 2087.202 * contr13 + 2.2353 * contr14 + 196.9057 * contr23 + 171.469 * contr24 - 7.2842 * contr34 + 2009.2051 ;
}

double DistanceEstimator::LM853()
{
	return -352.8373 * diff12 - 1058.9504 * diff13 - 1619.0706 * diff14 + 2434.1222 * diff23 + 2.6997 * diff24 + 438.6496 * diff34 + 1960.9991 * contr12 - 136.8325 * contr13 + 2.2353 * contr14 + 196.9057 * contr23 + 171.469 * contr24 - 7.2842 * contr34 + 1758.8203 ;
}

double DistanceEstimator::LM854()
{
	return 864.9737 * diff12 - 1058.9504 * diff13 - 4.6684 * diff14 + 2316.7736 * diff23 + 2.6997 * diff24 + 438.6496 * diff34 + 653.7086 * contr12 - 260.7766 * contr13 + 2.2353 * contr14 + 196.9057 * contr23 + 171.469 * contr24 - 7.2842 * contr34 + 1348.2278 ;
}

double DistanceEstimator::LM855()
{
	return 864.9737 * diff12 - 1058.9504 * diff13 - 4.6684 * diff14 + 2316.7736 * diff23 + 2.6997 * diff24 + 438.6496 * diff34 + 653.7086 * contr12 - 260.7766 * contr13 + 2.2353 * contr14 + 196.9057 * contr23 + 171.469 * contr24 - 7.2842 * contr34 + 1339.6786 ;
}

double DistanceEstimator::LM856()
{
	return -1570.2018 * diff12 - 2094.8353 * diff13 - 391.8385 * diff14 + 3198.425 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 - 220.9595 * contr12 - 1.8074 * contr13 + 2.2353 * contr14 + 455.9702 * contr23 + 373.5697 * contr24 - 7.2842 * contr34 + 1869.6874 ;
}

double DistanceEstimator::LM857()
{
	return -1570.2018 * diff12 - 2054.5865 * diff13 - 391.8385 * diff14 + 662.7575 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 + 215.6435 * contr12 + 758.2817 * contr13 + 2.2353 * contr14 + 455.9702 * contr23 + 373.5697 * contr24 - 7.2842 * contr34 + 2114.3294 ;
}

double DistanceEstimator::LM858()
{
	return -1570.2018 * diff12 - 2054.5865 * diff13 - 391.8385 * diff14 + 662.7575 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 + 215.6435 * contr12 + 800.5964 * contr13 + 2.2353 * contr14 + 455.9702 * contr23 + 373.5697 * contr24 - 7.2842 * contr34 + 2109.5313 ;
}

double DistanceEstimator::LM859()
{
	return -1570.2018 * diff12 - 2054.5865 * diff13 - 391.8385 * diff14 + 662.7575 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 + 215.6435 * contr12 - 3.3839 * contr13 + 2.2353 * contr14 + 455.9702 * contr23 + 373.5697 * contr24 - 7.2842 * contr34 + 2249.8204 ;
}

double DistanceEstimator::LM860()
{
	return -6421.0363 * diff12 - 1491.1039 * diff13 - 391.8385 * diff14 - 1197.0744 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 + 502.1841 * contr12 - 1.4916 * contr13 + 2.2353 * contr14 + 455.9702 * contr23 + 373.5697 * contr24 - 7.2842 * contr34 + 2904.7147 ;
}

double DistanceEstimator::LM861()
{
	return -5745.8121 * diff12 - 1491.1039 * diff13 - 391.8385 * diff14 - 1197.0744 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 + 502.1841 * contr12 + 38.0516 * contr13 + 2.2353 * contr14 + 455.9702 * contr23 + 373.5697 * contr24 - 7.2842 * contr34 + 2840.4076 ;
}

double DistanceEstimator::LM862()
{
	return -4446.5995 * diff12 - 1491.1039 * diff13 - 391.8385 * diff14 - 1197.0744 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 + 502.1841 * contr12 - 200.5267 * contr13 + 2.2353 * contr14 + 455.9702 * contr23 + 373.5697 * contr24 - 7.2842 * contr34 + 2727.9583 ;
}

double DistanceEstimator::LM863()
{
	return 695.7554 * diff12 - 5442.9784 * diff13 - 821.3206 * diff14 - 851.8667 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 - 210.3443 * contr12 + 2387.3551 * contr13 + 2.2353 * contr14 - 2674.7878 * contr23 + 274.9132 * contr24 - 7.2842 * contr34 + 3405.2145 ;
}

double DistanceEstimator::LM864()
{
	return 1087.1416 * diff12 - 4652.3664 * diff13 - 821.3206 * diff14 - 851.8667 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 - 1047.896 * contr12 - 25.4547 * contr13 + 2.2353 * contr14 - 347.1128 * contr23 + 274.9132 * contr24 - 7.2842 * contr34 + 3494.3495 ;
}

double DistanceEstimator::LM865()
{
	return 1087.1416 * diff12 - 4652.3664 * diff13 - 821.3206 * diff14 - 851.8667 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 - 992.1412 * contr12 - 25.4547 * contr13 + 2.2353 * contr14 - 347.1128 * contr23 + 274.9132 * contr24 - 7.2842 * contr34 + 3476.5946 ;
}

double DistanceEstimator::LM866()
{
	return 2558.7404 * diff12 - 5388.6861 * diff13 - 299.7148 * diff14 - 851.8667 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 - 1658.6645 * contr12 - 25.4547 * contr13 + 2.2353 * contr14 + 746.3858 * contr23 + 442.381 * contr24 - 7.2842 * contr34 + 3351.7139 ;
}

double DistanceEstimator::LM867()
{
	return 869.1058 * diff12 - 5275.6704 * diff13 - 299.7148 * diff14 - 851.8667 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 - 1658.6645 * contr12 - 25.4547 * contr13 + 2.2353 * contr14 + 746.3858 * contr23 + 2528.8097 * contr24 - 7.2842 * contr34 + 3030.4938 ;
}

double DistanceEstimator::LM868()
{
	return -2093.9481 * diff12 - 5275.6704 * diff13 - 299.7148 * diff14 - 851.8667 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 - 1658.6645 * contr12 - 25.4547 * contr13 + 2.2353 * contr14 + 746.3858 * contr23 + 1627.9507 * contr24 - 7.2842 * contr34 + 3648.2514 ;
}

double DistanceEstimator::LM869()
{
	return 2697.03 * diff12 - 3095.0248 * diff13 - 402.1959 * diff14 - 851.8667 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 - 3570.0243 * contr12 + 96.3492 * contr13 + 2.2353 * contr14 + 566.8304 * contr23 + 370.3309 * contr24 - 7.2842 * contr34 + 2806.9321 ;
}

double DistanceEstimator::LM870()
{
	return 2697.03 * diff12 - 3095.0248 * diff13 - 402.1959 * diff14 - 121.3743 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 - 1219.6683 * contr12 + 157.2511 * contr13 + 2.2353 * contr14 + 566.8304 * contr23 + 370.3309 * contr24 - 7.2842 * contr34 + 2364.983 ;
}

double DistanceEstimator::LM871()
{
	return 649.1915 * diff12 - 3095.0248 * diff13 - 402.1959 * diff14 + 997.4353 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 - 1219.6683 * contr12 + 2873.5752 * contr13 + 2.2353 * contr14 + 1648.8834 * contr23 + 370.3309 * contr24 - 7.2842 * contr34 + 1954.7394 ;
}

double DistanceEstimator::LM872()
{
	return 262.7435 * diff12 - 3095.0248 * diff13 - 402.1959 * diff14 + 997.4353 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 - 1219.6683 * contr12 + 2701.1661 * contr13 + 2.2353 * contr14 + 1648.8834 * contr23 + 370.3309 * contr24 - 7.2842 * contr34 + 2046.864 ;
}

double DistanceEstimator::LM873()
{
	return 2384.5588 * diff12 - 3095.0248 * diff13 - 402.1959 * diff14 + 1424.3848 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 - 1219.6683 * contr12 + 1144.0032 * contr13 + 2.2353 * contr14 + 1097.2485 * contr23 + 370.3309 * contr24 - 7.2842 * contr34 + 2001.9021 ;
}

double DistanceEstimator::LM874()
{
	return 2384.5588 * diff12 - 1457.4547 * diff13 - 402.1959 * diff14 + 787.3007 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 - 1219.6683 * contr12 + 508.2388 * contr13 + 2.2353 * contr14 + 1097.2485 * contr23 + 370.3309 * contr24 - 7.2842 * contr34 + 1731.3348 ;
}

double DistanceEstimator::LM875()
{
	return 2819.0269 * diff12 - 3095.0248 * diff13 - 402.1959 * diff14 + 787.3007 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 - 1219.6683 * contr12 + 508.2388 * contr13 + 2.2353 * contr14 + 1097.2485 * contr23 + 370.3309 * contr24 - 7.2842 * contr34 + 2156.7597 ;
}

double DistanceEstimator::LM876()
{
	return 2933.3606 * diff12 - 3095.0248 * diff13 - 402.1959 * diff14 + 787.3007 * diff23 + 2.6997 * diff24 + 322.6657 * diff34 - 1219.6683 * contr12 + 508.2388 * contr13 + 2.2353 * contr14 + 1097.2485 * contr23 + 370.3309 * contr24 - 7.2842 * contr34 + 2142.2588 ;
}

double DistanceEstimator::LM877()
{
	return 4.7242 * diff12 - 164.6767 * diff13 - 1069.6353 * diff14 + 9469.6781 * diff23 + 2.6997 * diff24 + 125.2464 * diff34 + 9238.4281 * contr12 - 6171.3943 * contr13 + 2.2353 * contr14 + 6309.3014 * contr23 + 47.5412 * contr24 - 7.2842 * contr34 + 50.0357 ;
}

double DistanceEstimator::LM878()
{
	return 3547.2468 * diff12 - 243.6207 * diff13 - 990.118 * diff14 - 172.6235 * diff23 + 2.6997 * diff24 + 117.0717 * diff34 - 3075.5588 * contr12 - 31.3228 * contr13 + 2.2353 * contr14 - 258.9478 * contr23 + 2.0297 * contr24 - 7.2842 * contr34 + 2017.6415 ;
}

double DistanceEstimator::LM879()
{
	return 4058.3987 * diff12 - 243.6207 * diff13 - 990.118 * diff14 - 172.6235 * diff23 + 2.6997 * diff24 + 117.0717 * diff34 - 3225.8538 * contr12 - 31.3228 * contr13 + 2.2353 * contr14 - 258.9478 * contr23 + 2.0297 * contr24 - 7.2842 * contr34 + 1968.0186 ;
}

double DistanceEstimator::LM880()
{
	return 4385.6533 * diff12 - 243.6207 * diff13 - 990.118 * diff14 - 172.6235 * diff23 + 2.6997 * diff24 + 117.0717 * diff34 - 2837.3357 * contr12 - 31.3228 * contr13 + 2.2353 * contr14 - 258.9478 * contr23 + 2.0297 * contr24 - 7.2842 * contr34 + 1889.3972 ;
}

double DistanceEstimator::LM881()
{
	return 787.4141 * diff12 - 243.6207 * diff13 - 5412.5145 * diff14 - 172.6235 * diff23 + 2.6997 * diff24 + 117.0717 * diff34 - 613.4556 * contr12 - 31.3228 * contr13 + 2.2353 * contr14 + 1565.0845 * contr23 + 2.0297 * contr24 - 7.2842 * contr34 + 2886.2906 ;
}

double DistanceEstimator::LM882()
{
	return 2782.5385 * diff12 - 243.6207 * diff13 + 1256.8252 * diff14 - 172.6235 * diff23 + 2.6997 * diff24 + 117.0717 * diff34 + 4369.8447 * contr12 - 31.3228 * contr13 + 2.2353 * contr14 - 225.718 * contr23 + 961.5188 * contr24 - 7.2842 * contr34 + 548.3111 ;
}

double DistanceEstimator::LM883()
{
	return 3537.8211 * diff12 - 243.6207 * diff13 + 1563.8001 * diff14 - 172.6235 * diff23 + 2.6997 * diff24 + 117.0717 * diff34 + 3651.1214 * contr12 - 31.3228 * contr13 + 2.2353 * contr14 - 225.718 * contr23 + 961.5188 * contr24 - 7.2842 * contr34 + 474.7416 ;
}

double DistanceEstimator::LM884()
{
	return 3549.0941 * diff12 - 243.6207 * diff13 + 1487.0564 * diff14 - 172.6235 * diff23 + 2.6997 * diff24 + 117.0717 * diff34 + 3651.1214 * contr12 - 31.3228 * contr13 + 2.2353 * contr14 - 225.718 * contr23 + 961.5188 * contr24 - 7.2842 * contr34 + 494.412 ;
}

double DistanceEstimator::LM885()
{
	return 1827.4475 * diff12 - 243.6207 * diff13 + 918.4191 * diff14 - 1674.45 * diff23 + 2.6997 * diff24 + 117.0717 * diff34 + 22.7174 * contr12 - 31.3228 * contr13 + 2.2353 * contr14 - 225.718 * contr23 + 852.0034 * contr24 - 7.2842 * contr34 + 1612.6617 ;
}

double DistanceEstimator::LM886()
{
	return 1502.5079 * diff12 - 243.6207 * diff13 + 918.4191 * diff14 - 1674.45 * diff23 + 2.6997 * diff24 + 117.0717 * diff34 - 1446.9239 * contr12 - 31.3228 * contr13 + 2.2353 * contr14 - 225.718 * contr23 + 1004.5921 * contr24 - 7.2842 * contr34 + 1804.6196 ;
}

double DistanceEstimator::LM887()
{
	return 3148.8571 * diff12 - 243.6207 * diff13 + 918.4191 * diff14 - 3197.9403 * diff23 + 2.6997 * diff24 + 117.0717 * diff34 - 2590.3224 * contr12 - 31.3228 * contr13 + 2.2353 * contr14 - 225.718 * contr23 + 593.7765 * contr24 - 7.2842 * contr34 + 1998.8376 ;
}

double DistanceEstimator::LM888()
{
	return 1691.4289 * diff12 - 243.6207 * diff13 + 1012.5438 * diff14 - 7850.9054 * diff23 + 2.6997 * diff24 + 117.0717 * diff34 + 48.9486 * contr12 - 31.3228 * contr13 + 2.2353 * contr14 - 225.718 * contr23 + 573.193 * contr24 - 7.2842 * contr34 + 2482.0615 ;
}

double DistanceEstimator::LM889()
{
	return 2186.0885 * diff12 - 219.2423 * diff13 - 5878.3817 * diff14 - 143.9746 * diff23 + 2.6997 * diff24 + 122.6361 * diff34 + 170.2913 * contr12 - 244.6632 * contr13 + 147.5382 * contr14 - 1090.83 * contr23 + 2.0297 * contr24 + 2788.6838 * contr34 + 1977.5005 ;
}

double DistanceEstimator::LM890()
{
	return 972.8757 * diff12 - 219.2423 * diff13 - 3081.0732 * diff14 - 254.7879 * diff23 + 2.6997 * diff24 + 122.6361 * diff34 + 977.5716 * contr12 - 425.9122 * contr13 - 350.116 * contr14 - 783.2828 * contr23 + 2.0297 * contr24 + 11.8545 * contr34 + 2372.4917 ;
}

double DistanceEstimator::LM891()
{
	return 1082.0268 * diff12 - 219.2423 * diff13 - 1268.3817 * diff14 - 2740.7005 * diff23 + 2.6997 * diff24 + 122.6361 * diff34 + 738.948 * contr12 - 425.9122 * contr13 - 977.9245 * contr14 - 3885.9181 * contr23 + 2.0297 * contr24 + 11.8545 * contr34 + 2791.3753 ;
}

double DistanceEstimator::LM892()
{
	return 657.9061 * diff12 - 219.2423 * diff13 - 1268.3817 * diff14 - 2575.1204 * diff23 + 2.6997 * diff24 + 1317.8342 * diff34 + 738.948 * contr12 - 425.9122 * contr13 - 417.4436 * contr14 - 5318.439 * contr23 + 2.0297 * contr24 + 11.8545 * contr34 + 2720.8537 ;
}

double DistanceEstimator::LM893()
{
	return 657.9061 * diff12 + 883.9329 * diff13 - 1268.3817 * diff14 - 2575.1204 * diff23 + 2.6997 * diff24 + 1185.0344 * diff34 + 738.948 * contr12 - 425.9122 * contr13 - 417.4436 * contr14 - 5131.4374 * contr23 + 2.0297 * contr24 + 11.8545 * contr34 + 2344.5322 ;
}

double DistanceEstimator::LM894()
{
	return 657.9061 * diff12 + 1116.1803 * diff13 - 1268.3817 * diff14 - 2575.1204 * diff23 + 2.6997 * diff24 + 1185.0344 * diff34 + 738.948 * contr12 - 425.9122 * contr13 - 417.4436 * contr14 - 5131.4374 * contr23 + 2.0297 * contr24 + 11.8545 * contr34 + 2280.5764 ;
}

double DistanceEstimator::LM895()
{
	return 657.9061 * diff12 - 219.2423 * diff13 - 1268.3817 * diff14 - 1762.5504 * diff23 + 2.6997 * diff24 + 122.6361 * diff34 + 738.948 * contr12 - 425.9122 * contr13 - 559.3796 * contr14 - 2255.9244 * contr23 + 2.0297 * contr24 + 11.8545 * contr34 + 2504.2872 ;
}

double DistanceEstimator::LM896()
{
	return 759.9787 * diff12 - 219.2423 * diff13 - 1253.4957 * diff14 - 20.0545 * diff23 + 2.6997 * diff24 + 122.6361 * diff34 - 10.9962 * contr12 - 433.2501 * contr13 + 187.6907 * contr14 - 34.2892 * contr23 + 2.0297 * contr24 + 11.8545 * contr34 + 1883.4285 ;
}

double DistanceEstimator::LM897()
{
	return 759.9787 * diff12 - 219.2423 * diff13 - 1253.4957 * diff14 - 20.0545 * diff23 + 2.6997 * diff24 + 122.6361 * diff34 - 10.9962 * contr12 - 462.7624 * contr13 + 187.6907 * contr14 - 34.2892 * contr23 + 2.0297 * contr24 + 11.8545 * contr34 + 1878.2464 ;
}

double DistanceEstimator::LM898()
{
	return 2831.1433 * diff12 - 219.2423 * diff13 - 1253.4957 * diff14 - 20.0545 * diff23 + 2.6997 * diff24 + 122.6361 * diff34 - 10.9962 * contr12 - 1659.0073 * contr13 + 187.6907 * contr14 - 10.9508 * contr23 + 2.0297 * contr24 + 11.8545 * contr34 + 1997.0669 ;
}

double DistanceEstimator::LM899()
{
	return 213.9819 * diff12 - 193.2514 * diff13 + 29.92 * diff14 - 348.3177 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 164.0572 * contr12 - 1297.4876 * contr13 + 29.5212 * contr14 - 303.5164 * contr23 + 2.0297 * contr24 - 269.045 * contr34 + 2140.4759 ;
}

double DistanceEstimator::LM900()
{
	return 500.9164 * diff12 - 193.2514 * diff13 + 29.92 * diff14 - 348.3177 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 164.0572 * contr12 - 2827.0752 * contr13 + 29.5212 * contr14 - 303.5164 * contr23 + 2.0297 * contr24 - 269.045 * contr34 + 2520.492 ;
}

double DistanceEstimator::LM901()
{
	return 537.0324 * diff12 - 193.2514 * diff13 + 29.92 * diff14 - 348.3177 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 164.0572 * contr12 - 2284.0092 * contr13 + 29.5212 * contr14 - 303.5164 * contr23 + 2.0297 * contr24 - 269.045 * contr34 + 2362.3323 ;
}

double DistanceEstimator::LM902()
{
	return 188.9499 * diff12 - 193.2514 * diff13 + 29.92 * diff14 - 348.3177 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 164.0572 * contr12 - 19.2052 * contr13 + 29.5212 * contr14 - 303.5164 * contr23 + 2.0297 * contr24 - 497.9561 * contr34 + 1840.743 ;
}

double DistanceEstimator::LM903()
{
	return 188.9499 * diff12 - 193.2514 * diff13 + 29.92 * diff14 - 348.3177 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 164.0572 * contr12 - 19.2052 * contr13 + 29.5212 * contr14 - 303.5164 * contr23 + 2.0297 * contr24 - 700.4543 * contr34 + 1877.2693 ;
}

double DistanceEstimator::LM904()
{
	return 192.1239 * diff12 - 193.2514 * diff13 + 293.0517 * diff14 + 26.0989 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 70.0513 * contr12 - 621.4935 * contr13 + 29.5212 * contr14 - 327.6573 * contr23 + 2.0297 * contr24 - 191.5275 * contr34 + 1712.9888 ;
}

double DistanceEstimator::LM905()
{
	return 192.1239 * diff12 - 193.2514 * diff13 + 323.7594 * diff14 + 26.0989 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 70.0513 * contr12 - 621.4935 * contr13 + 29.5212 * contr14 - 327.6573 * contr23 + 2.0297 * contr24 - 191.5275 * contr34 + 1709.1648 ;
}

double DistanceEstimator::LM906()
{
	return 192.1239 * diff12 - 193.2514 * diff13 + 124.1592 * diff14 + 212.0086 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 70.0513 * contr12 - 621.4935 * contr13 + 29.5212 * contr14 - 327.6573 * contr23 + 2.0297 * contr24 - 191.5275 * contr34 + 1732.2467 ;
}

double DistanceEstimator::LM907()
{
	return 958.5208 * diff12 + 456.4231 * diff13 + 732.7323 * diff14 - 836.8434 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 78.6459 * contr12 - 2215.6574 * contr13 + 29.5212 * contr14 - 611.3467 * contr23 + 2.0297 * contr24 - 191.5275 * contr34 + 1841.9514 ;
}

double DistanceEstimator::LM908()
{
	return 958.5208 * diff12 + 528.6092 * diff13 + 732.7323 * diff14 - 836.8434 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 78.6459 * contr12 - 2215.6574 * contr13 + 29.5212 * contr14 - 611.3467 * contr23 + 2.0297 * contr24 - 191.5275 * contr34 + 1821.4431 ;
}

double DistanceEstimator::LM909()
{
	return 1190.0328 * diff12 - 193.2514 * diff13 + 524.0786 * diff14 - 836.8434 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 78.6459 * contr12 - 2771.5031 * contr13 + 29.5212 * contr14 - 514.0817 * contr23 + 2.0297 * contr24 - 191.5275 * contr34 + 2190.6542 ;
}

double DistanceEstimator::LM910()
{
	return 182.3888 * diff12 - 193.2514 * diff13 + 304.0738 * diff14 - 244.0038 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 164.3896 * contr12 - 141.9076 * contr13 + 29.5212 * contr14 - 321.9854 * contr23 + 2.0297 * contr24 - 191.5275 * contr34 + 1687.227 ;
}

double DistanceEstimator::LM911()
{
	return 182.3888 * diff12 - 193.2514 * diff13 + 324.688 * diff14 - 244.0038 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 164.3896 * contr12 - 141.9076 * contr13 + 29.5212 * contr14 - 321.9854 * contr23 + 2.0297 * contr24 - 191.5275 * contr34 + 1689.5863 ;
}

double DistanceEstimator::LM912()
{
	return 182.3888 * diff12 - 193.2514 * diff13 + 252.0526 * diff14 - 244.0038 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 222.6075 * contr12 - 141.9076 * contr13 + 29.5212 * contr14 - 321.9854 * contr23 + 2.0297 * contr24 - 191.5275 * contr34 + 1705.7396 ;
}

double DistanceEstimator::LM913()
{
	return 4.7242 * diff12 - 644.2092 * diff13 - 1982.1525 * diff14 + 45.6712 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 208.6388 * contr12 - 283.2815 * contr13 + 730.4423 * contr14 - 102.0783 * contr23 + 2.0297 * contr24 - 101.8222 * contr34 + 2424.5166 ;
}

double DistanceEstimator::LM914()
{
	return 4.7242 * diff12 + 268.3935 * diff13 + 654.5054 * diff14 + 45.6712 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 155.8659 * contr12 - 2.5495 * contr13 - 127.5775 * contr14 - 102.0783 * contr23 + 2.0297 * contr24 - 101.8222 * contr34 + 1445.5728 ;
}

double DistanceEstimator::LM915()
{
	return 4.7242 * diff12 + 268.3935 * diff13 + 708.0943 * diff14 + 45.6712 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 155.8659 * contr12 - 2.5495 * contr13 - 127.5775 * contr14 - 102.0783 * contr23 + 2.0297 * contr24 - 101.8222 * contr34 + 1437.0209 ;
}

double DistanceEstimator::LM916()
{
	return 4.7242 * diff12 + 554.125 * diff13 + 427.9504 * diff14 + 45.6712 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 155.8659 * contr12 - 2.5495 * contr13 - 193.4576 * contr14 - 102.0783 * contr23 + 2.0297 * contr24 - 101.8222 * contr34 + 1416.0846 ;
}

double DistanceEstimator::LM917()
{
	return -3113.903 * diff12 - 4069.5976 * diff13 - 1651.2455 * diff14 - 7846.8305 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 22.3816 * contr12 - 621.1946 * contr13 + 4594.8134 * contr14 + 1307.0007 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 4729.1864 ;
}

double DistanceEstimator::LM918()
{
	return 4.7242 * diff12 - 3474.6663 * diff13 - 1669.9836 * diff14 - 803.0655 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 1561.2858 * contr12 - 502.6829 * contr13 + 625.4801 * contr14 - 607.8589 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 3549.6359 ;
}

double DistanceEstimator::LM919()
{
	return 4.7242 * diff12 - 3474.6663 * diff13 - 1669.9836 * diff14 - 979.666 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 301.9315 * contr12 - 502.6829 * contr13 + 625.4801 * contr14 + 354.8384 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 3316.0593 ;
}

double DistanceEstimator::LM920()
{
	return 4.7242 * diff12 - 3474.6663 * diff13 - 1724.3444 * diff14 - 803.0655 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 301.9315 * contr12 - 502.6829 * contr13 + 625.4801 * contr14 + 499.243 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 3294.3148 ;
}

double DistanceEstimator::LM921()
{
	return 4.7242 * diff12 - 3474.6663 * diff13 - 1669.9836 * diff14 - 803.0655 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 301.9315 * contr12 - 502.6829 * contr13 + 625.4801 * contr14 - 607.8589 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 3344.7136 ;
}

double DistanceEstimator::LM922()
{
	return 4.7242 * diff12 - 3474.6663 * diff13 - 1669.9836 * diff14 - 803.0655 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 354.7049 * contr12 - 502.6829 * contr13 + 625.4801 * contr14 - 607.8589 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 3345.7554 ;
}

double DistanceEstimator::LM923()
{
	return -1477.8221 * diff12 - 5630.9363 * diff13 - 1646.4323 * diff14 - 861.4712 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 22.3816 * contr12 - 502.6829 * contr13 + 625.4801 * contr14 - 539.0183 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 4157.6012 ;
}

double DistanceEstimator::LM924()
{
	return 4.7242 * diff12 - 3796.8145 * diff13 - 1995.8128 * diff14 + 705.4072 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 22.3816 * contr12 - 3090.8524 * contr13 - 355.7045 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 152.0582 * contr34 + 4210.1943 ;
}

double DistanceEstimator::LM925()
{
	return 4.7242 * diff12 - 3796.8145 * diff13 - 1699.2931 * diff14 + 705.4072 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 22.3816 * contr12 - 3090.8524 * contr13 - 334.6629 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 152.0582 * contr34 + 4128.4676 ;
}

double DistanceEstimator::LM926()
{
	return 4.7242 * diff12 - 3796.8145 * diff13 - 1699.2931 * diff14 + 705.4072 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 22.3816 * contr12 - 3090.8524 * contr13 - 334.6629 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 152.0582 * contr34 + 4126.8951 ;
}

double DistanceEstimator::LM927()
{
	return 4.7242 * diff12 - 4316.8042 * diff13 - 1345.9908 * diff14 + 705.4072 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 22.3816 * contr12 - 3090.8524 * contr13 - 165.1867 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 152.0582 * contr34 + 4158.095 ;
}

double DistanceEstimator::LM928()
{
	return 4.7242 * diff12 - 2796.8343 * diff13 - 1045.2592 * diff14 + 705.4072 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 22.3816 * contr12 - 3679.8921 * contr13 + 137.1896 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 152.0582 * contr34 + 3762.4369 ;
}

double DistanceEstimator::LM929()
{
	return 4.7242 * diff12 - 2796.8343 * diff13 - 821.4265 * diff14 + 705.4072 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 2283.0952 * contr12 - 2266.1969 * contr13 - 86.534 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 152.0582 * contr34 + 3908.8643 ;
}

double DistanceEstimator::LM930()
{
	return 759.8652 * diff12 + 751.1012 * diff13 - 1276.553 * diff14 + 3554.5055 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 228.0222 * contr12 - 5454.0204 * contr13 + 368.3707 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 139.8648 * contr34 + 2690.1254 ;
}

double DistanceEstimator::LM931()
{
	return -541.6753 * diff12 - 2796.8343 * diff13 - 1932.9908 * diff14 + 3478.7874 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 796.8239 * contr12 - 4192.1665 * contr13 + 368.3707 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 523.502 * contr34 + 3561.7083 ;
}

double DistanceEstimator::LM932()
{
	return -740.5586 * diff12 - 2796.8343 * diff13 - 2062.5716 * diff14 + 3478.7874 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 796.8239 * contr12 - 4192.1665 * contr13 + 368.3707 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 495.0845 * contr34 + 3612.3078 ;
}

double DistanceEstimator::LM933()
{
	return -740.5586 * diff12 - 2796.8343 * diff13 - 2079.1331 * diff14 + 3478.7874 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 796.8239 * contr12 - 4192.1665 * contr13 + 368.3707 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 495.0845 * contr34 + 3616.3444 ;
}

double DistanceEstimator::LM934()
{
	return -790.8361 * diff12 - 2796.8343 * diff13 - 1913.518 * diff14 + 3478.7874 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 796.8239 * contr12 - 4192.1665 * contr13 + 368.3707 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 495.0845 * contr34 + 3577.7837 ;
}

double DistanceEstimator::LM935()
{
	return -204.6559 * diff12 - 2796.8343 * diff13 - 2790.2085 * diff14 + 3478.7874 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 929.4754 * contr12 - 4192.1665 * contr13 + 368.3707 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 139.8648 * contr34 + 3648.2385 ;
}

double DistanceEstimator::LM936()
{
	return 475.1399 * diff12 - 2796.8343 * diff13 - 1453.3118 * diff14 + 4225.828 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 733.053 * contr12 - 4192.1665 * contr13 + 368.3707 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 139.8648 * contr34 + 3162.888 ;
}

double DistanceEstimator::LM937()
{
	return -924.9445 * diff12 - 7201.4619 * diff13 - 887.1784 * diff14 - 329.1049 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 3626.5633 * contr12 - 478.2758 * contr13 + 368.1766 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 5264.3745 ;
}

double DistanceEstimator::LM938()
{
	return -1616.7595 * diff12 - 4797.9493 * diff13 - 1248.3308 * diff14 - 329.1049 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 572.7421 * contr12 - 478.2758 * contr13 + 368.1766 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 3890.5743 ;
}

double DistanceEstimator::LM939()
{
	return -1616.7595 * diff12 - 4314.9027 * diff13 - 1449.4411 * diff14 - 329.1049 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 572.7421 * contr12 - 478.2758 * contr13 + 368.1766 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 3792.2981 ;
}

double DistanceEstimator::LM940()
{
	return -1616.7595 * diff12 - 4314.9027 * diff13 - 1462.6195 * diff14 - 329.1049 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 572.7421 * contr12 - 478.2758 * contr13 + 368.1766 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 3792.916 ;
}

double DistanceEstimator::LM941()
{
	return -2237.8237 * diff12 - 2745.001 * diff13 - 634.912 * diff14 - 329.1049 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 572.7421 * contr12 - 478.2758 * contr13 + 368.1766 * contr14 + 11.7061 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 3200.6889 ;
}

double DistanceEstimator::LM942()
{
	return 4.7242 * diff12 - 1802.1872 * diff13 + 11.5455 * diff14 + 2229.4225 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 783.6398 * contr12 - 411.9954 * contr13 + 93.4961 * contr14 - 2399.7348 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 1733.9626 ;
}

double DistanceEstimator::LM943()
{
	return 4.7242 * diff12 - 1802.1872 * diff13 + 916.4363 * diff14 + 4721.5058 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 783.6398 * contr12 - 411.9954 * contr13 + 93.4961 * contr14 - 1175.1832 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 998.1217 ;
}

double DistanceEstimator::LM944()
{
	return 4.7242 * diff12 - 1802.1872 * diff13 - 369.0451 * diff14 + 4087.6282 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 783.6398 * contr12 - 411.9954 * contr13 + 93.4961 * contr14 - 1175.1832 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 1454.1935 ;
}

double DistanceEstimator::LM945()
{
	return 4.7242 * diff12 - 2690.7824 * diff13 - 369.0451 * diff14 + 4087.6282 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 783.6398 * contr12 - 411.9954 * contr13 + 93.4961 * contr14 - 1175.1832 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 1702.8601 ;
}

double DistanceEstimator::LM946()
{
	return -1400.179 * diff12 - 1802.1872 * diff13 - 883.9618 * diff14 - 1051.1917 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 636.9649 * contr12 - 411.9954 * contr13 + 93.4961 * contr14 + 791.9426 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 2598.855 ;
}

double DistanceEstimator::LM947()
{
	return -1142.8786 * diff12 - 1802.1872 * diff13 - 883.9618 * diff14 - 1051.1917 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 636.9649 * contr12 - 411.9954 * contr13 + 93.4961 * contr14 + 791.9426 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 2560.8731 ;
}

double DistanceEstimator::LM948()
{
	return -339.6715 * diff12 - 725.2671 * diff13 + 130.3471 * diff14 - 1051.1917 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 + 178.5219 * contr12 - 411.9954 * contr13 + 93.4961 * contr14 - 724.0057 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 2093.5609 ;
}

double DistanceEstimator::LM949()
{
	return -339.6715 * diff12 - 462.9919 * diff13 - 342.997 * diff14 - 1051.1917 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 884.7265 * contr12 - 411.9954 * contr13 + 93.4961 * contr14 - 38.0215 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 2245.2094 ;
}

double DistanceEstimator::LM950()
{
	return -339.6715 * diff12 - 462.9919 * diff13 - 342.997 * diff14 - 1051.1917 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 774.9841 * contr12 - 411.9954 * contr13 + 93.4961 * contr14 - 38.0215 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 2228.8254 ;
}

double DistanceEstimator::LM951()
{
	return -339.6715 * diff12 - 926.2402 * diff13 - 108.7862 * diff14 - 1051.1917 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 137.086 * contr12 - 411.9954 * contr13 + 93.4961 * contr14 - 38.0215 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 2204.1776 ;
}

double DistanceEstimator::LM952()
{
	return -339.6715 * diff12 + 38.9622 * diff13 + 166.2904 * diff14 - 1051.1917 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 137.086 * contr12 - 411.9954 * contr13 + 93.4961 * contr14 - 38.0215 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 1861.8569 ;
}

double DistanceEstimator::LM953()
{
	return 4.7242 * diff12 - 1802.1872 * diff13 - 883.9618 * diff14 - 5698.0773 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 3830.6646 * contr12 - 2101.3526 * contr13 + 93.4961 * contr14 + 959.3984 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 4270.8067 ;
}

double DistanceEstimator::LM954()
{
	return 4.7242 * diff12 - 1802.1872 * diff13 - 883.9618 * diff14 - 5009.5567 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 1463.8695 * contr12 - 3191.2521 * contr13 + 93.4961 * contr14 + 959.3984 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 4013.2057 ;
}

double DistanceEstimator::LM955()
{
	return 4.7242 * diff12 - 1802.1872 * diff13 - 883.9618 * diff14 - 4813.4563 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 1369.286 * contr12 - 1843.2889 * contr13 + 93.4961 * contr14 + 959.3984 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 3683.3727 ;
}

double DistanceEstimator::LM956()
{
	return -3160.442 * diff12 - 270.2074 * diff13 - 74.1046 * diff14 + 38.4987 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 22.3816 * contr12 - 617.096 * contr13 - 726.2168 * contr14 - 78.7748 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 2429.9439 ;
}

double DistanceEstimator::LM957()
{
	return -657.2327 * diff12 - 270.2074 * diff13 - 74.1046 * diff14 + 38.4987 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 22.3816 * contr12 - 4135.5222 * contr13 - 682.5217 * contr14 - 78.7748 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 3164.7684 ;
}

double DistanceEstimator::LM958()
{
	return -767.4087 * diff12 - 270.2074 * diff13 - 74.1046 * diff14 + 38.4987 * diff23 + 2.6997 * diff24 + 65.9805 * diff34 - 22.3816 * contr12 - 2036.5267 * contr13 - 250.7593 * contr14 - 78.7748 * contr23 + 2.0297 * contr24 - 5.7369 * contr34 + 2438.2632 ;
}

double DistanceEstimator::LM959()
{
	return 339.2296 * diff12 - 1105.1338 * diff13 - 139.1681 * diff14 - 1997.4923 * diff23 + 2.6997 * diff24 + 478.2756 * diff34 - 58.7155 * contr12 - 386.6496 * contr13 + 269.9693 * contr14 - 49.4001 * contr23 + 2.0297 * contr24 - 31.9562 * contr34 + 2243.9602 ;
}

double DistanceEstimator::LM960()
{
	return 578.1621 * diff12 - 1105.1338 * diff13 - 607.4055 * diff14 - 1997.4923 * diff23 + 2.6997 * diff24 + 478.2756 * diff34 - 58.7155 * contr12 - 386.6496 * contr13 + 269.9693 * contr14 - 49.4001 * contr23 + 2.0297 * contr24 - 31.9562 * contr34 + 2331.6593 ;
}

double DistanceEstimator::LM961()
{
	return 578.1621 * diff12 - 1105.1338 * diff13 - 139.1681 * diff14 - 1997.4923 * diff23 + 2.6997 * diff24 + 478.2756 * diff34 - 58.7155 * contr12 - 386.6496 * contr13 + 269.9693 * contr14 - 49.4001 * contr23 + 2.0297 * contr24 - 31.9562 * contr34 + 2222.568 ;
}

double DistanceEstimator::LM962()
{
	return 4.7242 * diff12 - 1105.1338 * diff13 + 923.2303 * diff14 - 10652.7865 * diff23 + 2.6997 * diff24 + 478.2756 * diff34 - 58.7155 * contr12 - 870.3312 * contr13 + 810.8561 * contr14 - 49.4001 * contr23 + 2.0297 * contr24 - 31.9562 * contr34 + 3434.3104 ;
}

double DistanceEstimator::LM963()
{
	return 4.7242 * diff12 - 1105.1338 * diff13 + 532.7581 * diff14 - 9848.5557 * diff23 + 2.6997 * diff24 + 478.2756 * diff34 - 58.7155 * contr12 - 870.3312 * contr13 + 302.1269 * contr14 - 49.4001 * contr23 + 2.0297 * contr24 - 31.9562 * contr34 + 3517.1423 ;
}

double DistanceEstimator::LM964()
{
	return 4.7242 * diff12 - 1491.8376 * diff13 - 443.9023 * diff14 + 33.9678 * diff23 + 2.6997 * diff24 + 339.3611 * diff34 - 898.5264 * contr12 - 20.8254 * contr13 + 52.4356 * contr14 - 893.249 * contr23 + 2.0297 * contr24 - 217.2768 * contr34 + 2392.7077 ;
}

double DistanceEstimator::LM965()
{
	return 4.7242 * diff12 - 1491.8376 * diff13 - 443.9023 * diff14 + 33.9678 * diff23 + 2.6997 * diff24 + 339.3611 * diff34 - 739.5289 * contr12 - 20.8254 * contr13 + 52.4356 * contr14 - 796.2152 * contr23 + 2.0297 * contr24 - 143.1486 * contr34 + 2320.2989 ;
}

double DistanceEstimator::LM966()
{
	return 4.7242 * diff12 - 1491.8376 * diff13 - 443.9023 * diff14 + 33.9678 * diff23 + 2.6997 * diff24 + 339.3611 * diff34 - 696.251 * contr12 - 20.8254 * contr13 + 52.4356 * contr14 - 737.8992 * contr23 + 2.0297 * contr24 - 143.1486 * contr34 + 2313.3413 ;
}

double DistanceEstimator::LM967()
{
	return 4.7242 * diff12 - 1491.8376 * diff13 - 443.9023 * diff14 + 33.9678 * diff23 + 2.6997 * diff24 + 339.3611 * diff34 - 716.1799 * contr12 - 20.8254 * contr13 + 52.4356 * contr14 - 737.8992 * contr23 + 2.0297 * contr24 - 143.1486 * contr34 + 2316.3719 ;
}

double DistanceEstimator::LM968()
{
	return 4.7242 * diff12 - 1491.8376 * diff13 - 443.9023 * diff14 + 33.9678 * diff23 + 2.6997 * diff24 + 339.3611 * diff34 - 716.1799 * contr12 - 20.8254 * contr13 + 52.4356 * contr14 - 737.8992 * contr23 + 2.0297 * contr24 - 143.1486 * contr34 + 2315.003 ;
}

double DistanceEstimator::LM969()
{
	return 4.7242 * diff12 - 1652.9523 * diff13 - 530.2095 * diff14 + 33.9678 * diff23 + 2.6997 * diff24 + 339.3611 * diff34 - 171.3468 * contr12 - 20.8254 * contr13 + 92.107 * contr14 - 107.1759 * contr23 + 2.0297 * contr24 + 61.5681 * contr34 + 2160.7328 ;
}

double DistanceEstimator::LM970()
{
	return -587.3994 * diff12 - 1303.8581 * diff13 - 327.3719 * diff14 + 33.9678 * diff23 + 2.6997 * diff24 + 339.3611 * diff34 - 171.3468 * contr12 + 662.485 * contr13 - 228.1544 * contr14 - 107.1759 * contr23 + 2.0297 * contr24 + 10.3944 * contr34 + 1924.1508 ;
}

double DistanceEstimator::LM971()
{
	return 4.7242 * diff12 - 1303.8581 * diff13 - 327.3719 * diff14 + 33.9678 * diff23 + 2.6997 * diff24 + 339.3611 * diff34 - 171.3468 * contr12 + 30.053 * contr13 + 50.1534 * contr14 - 107.1759 * contr23 + 2.0297 * contr24 + 10.3944 * contr34 + 2017.5064 ;
}

double DistanceEstimator::LM972()
{
	return 4.7242 * diff12 - 1303.8581 * diff13 - 327.3719 * diff14 + 33.9678 * diff23 + 2.6997 * diff24 + 339.3611 * diff34 - 171.3468 * contr12 + 30.053 * contr13 + 50.1534 * contr14 - 107.1759 * contr23 + 2.0297 * contr24 + 10.3944 * contr34 + 2022.3986 ;
}

double DistanceEstimator::LM973()
{
	return 4.7242 * diff12 - 934.7863 * diff13 - 4.6684 * diff14 + 162.3678 * diff23 + 2.6997 * diff24 + 278.3988 * diff34 - 288.7312 * contr12 - 165.2272 * contr13 - 4.7127 * contr14 - 49.4001 * contr23 + 2.0297 * contr24 - 31.9562 * contr34 + 1932.4855 ;
}

double DistanceEstimator::LM974()
{
	return 4.7242 * diff12 - 934.7863 * diff13 - 4.6684 * diff14 + 162.3678 * diff23 + 2.6997 * diff24 + 278.3988 * diff34 - 288.7312 * contr12 - 165.2272 * contr13 - 4.7127 * contr14 - 49.4001 * contr23 + 2.0297 * contr24 - 31.9562 * contr34 + 1929.9717 ;
}

double DistanceEstimator::LM975()
{
	return 4.7242 * diff12 - 934.7863 * diff13 - 4.6684 * diff14 + 1411.805 * diff23 + 2.6997 * diff24 + 327.65 * diff34 - 288.7312 * contr12 - 165.2272 * contr13 - 4.7127 * contr14 - 49.4001 * contr23 + 2.0297 * contr24 - 31.9562 * contr34 + 1705.1501 ;
}

double DistanceEstimator::LM976()
{
	return 4.7242 * diff12 - 934.7863 * diff13 - 4.6684 * diff14 + 1411.805 * diff23 + 2.6997 * diff24 + 327.65 * diff34 - 288.7312 * contr12 - 165.2272 * contr13 - 4.7127 * contr14 - 49.4001 * contr23 + 2.0297 * contr24 - 31.9562 * contr34 + 1703.7664 ;
}

double DistanceEstimator::LM977()
{
	return 4.7242 * diff12 - 934.7863 * diff13 - 4.6684 * diff14 + 1411.805 * diff23 + 2.6997 * diff24 + 327.65 * diff34 - 288.7312 * contr12 - 165.2272 * contr13 - 4.7127 * contr14 - 49.4001 * contr23 + 2.0297 * contr24 - 31.9562 * contr34 + 1708.451 ;
}

double DistanceEstimator::LM978()
{
	return 4.7242 * diff12 - 934.7863 * diff13 - 4.6684 * diff14 + 162.3678 * diff23 + 2.6997 * diff24 + 422.6345 * diff34 + 124.6962 * contr12 - 181.246 * contr13 - 4.7127 * contr14 - 49.4001 * contr23 + 2.0297 * contr24 - 31.9562 * contr34 + 1815.9198 ;
}

double DistanceEstimator::LM979()
{
	return 4.7242 * diff12 - 934.7863 * diff13 + 134.3358 * diff14 + 162.3678 * diff23 + 2.6997 * diff24 + 422.6345 * diff34 + 604.6776 * contr12 - 181.246 * contr13 - 4.7127 * contr14 - 49.4001 * contr23 + 2.0297 * contr24 - 31.9562 * contr34 + 1683.5762 ;
}

double DistanceEstimator::LM980()
{
	return 4.7242 * diff12 - 934.7863 * diff13 + 191.2011 * diff14 + 162.3678 * diff23 + 2.6997 * diff24 + 422.6345 * diff34 + 450.9436 * contr12 - 181.246 * contr13 - 4.7127 * contr14 - 49.4001 * contr23 + 2.0297 * contr24 - 31.9562 * contr34 + 1706.8286 ;
}

double DistanceEstimator::LM981()
{
	return -3151.6236 * diff12 - 581.4138 * diff13 - 4.6684 * diff14 + 13.0283 * diff23 + 2.6997 * diff24 + 1499.8896 * diff34 - 2055.4813 * contr12 - 2443.2642 * contr13 - 23.7255 * contr14 - 49.4001 * contr23 + 2.0297 * contr24 - 690.0336 * contr34 + 3331.8996 ;
}

double DistanceEstimator::LM982()
{
	return -528.0305 * diff12 - 88.2401 * diff13 - 184.2042 * diff14 + 5866.0709 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 510.1092 * contr12 - 825.4812 * contr13 + 185.0811 * contr14 + 614.1306 * contr23 + 2.0297 * contr24 - 118.9611 * contr34 + 1063.755 ;
}

double DistanceEstimator::LM983()
{
	return -596.3324 * diff12 - 88.2401 * diff13 - 184.2042 * diff14 + 3706.3936 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 534.9886 * contr12 - 443.0791 * contr13 + 185.0811 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 118.9611 * contr34 + 1405.2042 ;
}

double DistanceEstimator::LM984()
{
	return -596.3324 * diff12 - 88.2401 * diff13 - 184.2042 * diff14 + 3706.3936 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 416.8276 * contr12 - 323.9986 * contr13 + 185.0811 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 118.9611 * contr34 + 1178.395 ;
}

double DistanceEstimator::LM985()
{
	return -596.3324 * diff12 - 88.2401 * diff13 - 184.2042 * diff14 + 3706.3936 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 677.1087 * contr12 - 397.7516 * contr13 + 185.0811 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 118.9611 * contr34 + 1152.7381 ;
}

double DistanceEstimator::LM986()
{
	return -338.8431 * diff12 - 979.5944 * diff13 - 1031.1406 * diff14 + 4720.5972 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1288.1047 * contr12 - 278.6784 * contr13 + 185.0811 * contr14 + 167.7339 * contr23 + 2.0297 * contr24 - 25.3328 * contr34 + 1696.2138 ;
}

double DistanceEstimator::LM987()
{
	return -338.8431 * diff12 - 1528.7232 * diff13 - 1031.1406 * diff14 + 4720.5972 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1288.1047 * contr12 - 278.6784 * contr13 + 185.0811 * contr14 + 167.7339 * contr23 + 2.0297 * contr24 - 25.3328 * contr34 + 1848.5007 ;
}

double DistanceEstimator::LM988()
{
	return -338.8431 * diff12 - 1559.2303 * diff13 - 1031.1406 * diff14 + 4720.5972 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1288.1047 * contr12 - 278.6784 * contr13 + 185.0811 * contr14 + 167.7339 * contr23 + 2.0297 * contr24 - 25.3328 * contr34 + 1856.5385 ;
}

double DistanceEstimator::LM989()
{
	return -416.9265 * diff12 - 979.5944 * diff13 - 1031.1406 * diff14 + 4720.5972 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1288.1047 * contr12 - 278.6784 * contr13 + 185.0811 * contr14 + 167.7339 * contr23 + 2.0297 * contr24 - 25.3328 * contr34 + 1700.6838 ;
}

double DistanceEstimator::LM990()
{
	return 4.7242 * diff12 - 979.5944 * diff13 - 849.6354 * diff14 + 6664.6211 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1288.1047 * contr12 - 278.6784 * contr13 + 185.0811 * contr14 + 319.2905 * contr23 + 2.0297 * contr24 - 25.3328 * contr34 + 1256.9348 ;
}

double DistanceEstimator::LM991()
{
	return 4.7242 * diff12 - 979.5944 * diff13 - 636.9177 * diff14 + 6908.0265 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1288.1047 * contr12 - 278.6784 * contr13 + 185.0811 * contr14 + 319.2905 * contr23 + 2.0297 * contr24 - 25.3328 * contr34 + 1161.4388 ;
}

double DistanceEstimator::LM992()
{
	return 4.7242 * diff12 - 979.5944 * diff13 - 1341.0369 * diff14 + 2989.8576 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 944.5254 * contr12 - 278.6784 * contr13 + 185.0811 * contr14 - 324.825 * contr23 + 2.0297 * contr24 - 25.3328 * contr34 + 2012.0991 ;
}

double DistanceEstimator::LM993()
{
	return 4.7242 * diff12 + 1459.5648 * diff13 - 1341.0369 * diff14 + 2989.8576 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 944.5254 * contr12 - 1392.4443 * contr13 + 185.0811 * contr14 - 324.825 * contr23 + 2.0297 * contr24 - 25.3328 * contr34 + 1648.9961 ;
}

double DistanceEstimator::LM994()
{
	return 4.7242 * diff12 - 979.5944 * diff13 - 1052.599 * diff14 + 2989.8576 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1466.2319 * contr12 - 278.6784 * contr13 + 185.0811 * contr14 - 621.2788 * contr23 + 2.0297 * contr24 - 25.3328 * contr34 + 2058.757 ;
}

double DistanceEstimator::LM995()
{
	return 4.7242 * diff12 - 979.5944 * diff13 - 868.9775 * diff14 + 2989.8576 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1863.7226 * contr12 - 278.6784 * contr13 + 185.0811 * contr14 - 847.1483 * contr23 + 2.0297 * contr24 - 25.3328 * contr34 + 2103.2145 ;
}

double DistanceEstimator::LM996()
{
	return -1636.9947 * diff12 - 1834.5669 * diff13 - 1338.415 * diff14 + 2083.9511 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 208.2841 * contr12 + 7.2283 * contr13 + 185.0811 * contr14 - 634.1749 * contr23 + 2.0297 * contr24 - 452.2959 * contr34 + 2528.4463 ;
}

double DistanceEstimator::LM997()
{
	return 772.6839 * diff12 - 1834.5669 * diff13 - 1338.415 * diff14 + 2083.9511 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 208.2841 * contr12 + 7.2283 * contr13 + 185.0811 * contr14 - 634.1749 * contr23 + 2.0297 * contr24 - 840.4442 * contr34 + 2348.3577 ;
}

double DistanceEstimator::LM998()
{
	return 657.4899 * diff12 - 1834.5669 * diff13 - 1338.415 * diff14 + 2083.9511 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 208.2841 * contr12 + 7.2283 * contr13 + 185.0811 * contr14 - 634.1749 * contr23 + 2.0297 * contr24 - 840.4442 * contr34 + 2362.2087 ;
}

double DistanceEstimator::LM999()
{
	return 4.7242 * diff12 + 106.2792 * diff13 - 60.8154 * diff14 + 2161.7179 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1158.2165 * contr12 + 487.0713 * contr13 + 100.25 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 272.9903 * contr34 + 1349.6018 ;
}

double DistanceEstimator::LM1000()
{
	return 4.7242 * diff12 + 106.2792 * diff13 - 202.7776 * diff14 + 2161.7179 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1158.2165 * contr12 + 487.0713 * contr13 + 100.25 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 272.9903 * contr34 + 1388.6535 ;
}

double DistanceEstimator::LM1001()
{
	return 1052.5387 * diff12 + 106.2792 * diff13 - 588.1036 * diff14 + 2161.7179 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1088.799 * contr12 - 392.8279 * contr13 + 100.25 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 272.9903 * contr34 + 1609.2506 ;
}

double DistanceEstimator::LM1002()
{
	return 2023.7439 * diff12 - 1434.7352 * diff13 - 588.1036 * diff14 + 2161.7179 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1088.799 * contr12 - 672.4307 * contr13 + 100.25 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 272.9903 * contr34 + 2035.6194 ;
}

double DistanceEstimator::LM1003()
{
	return 2106.914 * diff12 - 1353.003 * diff13 - 588.1036 * diff14 + 2161.7179 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1088.799 * contr12 - 672.4307 * contr13 + 100.25 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 272.9903 * contr34 + 2004.8644 ;
}

double DistanceEstimator::LM1004()
{
	return 1939.5185 * diff12 - 1185.5722 * diff13 - 588.1036 * diff14 + 2161.7179 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1088.799 * contr12 - 844.3045 * contr13 + 100.25 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 272.9903 * contr34 + 2018.7587 ;
}

double DistanceEstimator::LM1005()
{
	return -3546.0057 * diff12 - 1941.5918 * diff13 - 489.2882 * diff14 + 2161.7179 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 1738.4483 * contr12 + 841.3866 * contr13 + 100.25 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 272.9903 * contr34 + 1690.8298 ;
}

double DistanceEstimator::LM1006()
{
	return -1235.8313 * diff12 - 3170.3144 * diff13 - 489.2882 * diff14 + 2161.7179 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 1272.0389 * contr12 + 841.3866 * contr13 + 100.25 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 272.9903 * contr34 + 1909.0331 ;
}

double DistanceEstimator::LM1007()
{
	return -1235.8313 * diff12 - 3170.3144 * diff13 - 489.2882 * diff14 + 2161.7179 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 1294.9931 * contr12 + 841.3866 * contr13 + 100.25 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 272.9903 * contr34 + 1911.9689 ;
}

double DistanceEstimator::LM1008()
{
	return 4.7242 * diff12 + 106.2792 * diff13 - 603.3373 * diff14 + 2161.7179 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 1046.5906 * contr12 + 974.3951 * contr13 - 82.2115 * contr14 + 880.8195 * contr23 + 2.0297 * contr24 + 272.9903 * contr34 + 879.5428 ;
}

double DistanceEstimator::LM1009()
{
	return 4.7242 * diff12 + 106.2792 * diff13 - 647.8937 * diff14 + 2161.7179 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 1236.5174 * contr12 + 974.3951 * contr13 - 82.2115 * contr14 + 880.8195 * contr23 + 2.0297 * contr24 + 272.9903 * contr34 + 859.499 ;
}

double DistanceEstimator::LM1010()
{
	return 4.7242 * diff12 + 854.8413 * diff13 - 907.9665 * diff14 + 2161.7179 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 896.6206 * contr12 + 974.3951 * contr13 - 82.2115 * contr14 + 647.0496 * contr23 + 2.0297 * contr24 + 272.9903 * contr34 + 810.65 ;
}

double DistanceEstimator::LM1011()
{
	return 4.7242 * diff12 + 824.8988 * diff13 - 907.9665 * diff14 + 2161.7179 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 896.6206 * contr12 + 974.3951 * contr13 - 82.2115 * contr14 + 647.0496 * contr23 + 2.0297 * contr24 + 272.9903 * contr34 + 823.7302 ;
}

double DistanceEstimator::LM1012()
{
	return 4.7242 * diff12 + 404.0127 * diff13 + 825.6315 * diff14 + 5003.1677 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 17.5931 * contr12 + 462.1828 * contr13 + 336.6452 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 531.0497 * contr34 + 243.6385 ;
}

double DistanceEstimator::LM1013()
{
	return 4.7242 * diff12 + 404.0127 * diff13 + 703.5325 * diff14 + 5003.1677 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 17.5931 * contr12 + 462.1828 * contr13 + 336.6452 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 531.0497 * contr34 + 279.2997 ;
}

double DistanceEstimator::LM1014()
{
	return 4.7242 * diff12 + 404.0127 * diff13 + 431.9827 * diff14 + 5872.5196 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 17.5931 * contr12 + 462.1828 * contr13 + 336.6452 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 531.0497 * contr34 + 187.3916 ;
}

double DistanceEstimator::LM1015()
{
	return 4850.6866 * diff12 + 2271.852 * diff13 + 122.4205 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 663.424 * contr12 - 1270.9159 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 583.751 * contr34 + 809.5112 ;
}

double DistanceEstimator::LM1016()
{
	return 4.7242 * diff12 - 1294.2785 * diff13 + 122.4205 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1071.0325 * contr12 - 1270.9159 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 583.751 * contr34 + 2450.1631 ;
}

double DistanceEstimator::LM1017()
{
	return -251.0197 * diff12 + 440.7076 * diff13 + 122.4205 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1043.8586 * contr12 - 1270.9159 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 583.751 * contr34 + 1937.5266 ;
}

double DistanceEstimator::LM1018()
{
	return 4.7242 * diff12 + 440.7076 * diff13 + 122.4205 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1043.8586 * contr12 - 1270.9159 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 583.751 * contr34 + 1905.6879 ;
}

double DistanceEstimator::LM1019()
{
	return 4.7242 * diff12 + 440.7076 * diff13 + 122.4205 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 1043.8586 * contr12 - 1270.9159 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 583.751 * contr34 + 1908.2902 ;
}

double DistanceEstimator::LM1020()
{
	return 414.1468 * diff12 + 331.7019 * diff13 + 122.4205 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 372.4631 * contr12 - 802.0751 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 497.9905 * contr34 + 1590.9853 ;
}

double DistanceEstimator::LM1021()
{
	return 414.1468 * diff12 + 293.4035 * diff13 + 122.4205 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 372.4631 * contr12 - 802.0751 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 497.9905 * contr34 + 1604.3953 ;
}

double DistanceEstimator::LM1022()
{
	return 414.1468 * diff12 - 472.5643 * diff13 + 122.4205 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 372.4631 * contr12 - 802.0751 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 497.9905 * contr34 + 1824.3151 ;
}

double DistanceEstimator::LM1023()
{
	return 1024.0992 * diff12 - 472.5643 * diff13 + 408.2709 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 465.2149 * contr12 - 1423.9071 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 375.5732 * contr34 + 1900.3051 ;
}

double DistanceEstimator::LM1024()
{
	return 712.899 * diff12 - 472.5643 * diff13 + 142.5357 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 380.3038 * contr12 - 1106.6459 * contr13 - 11.4462 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 375.5732 * contr34 + 1876.0365 ;
}

double DistanceEstimator::LM1025()
{
	return 712.899 * diff12 - 472.5643 * diff13 + 33.542 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 303.1119 * contr12 - 1106.6459 * contr13 - 69.521 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 375.5732 * contr34 + 1881.2984 ;
}

double DistanceEstimator::LM1026()
{
	return 964.3085 * diff12 - 472.5643 * diff13 + 122.4205 * diff14 + 8422.3253 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 579.3711 * contr12 - 488.0382 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 45.4965 * contr34 + 299.1516 ;
}

double DistanceEstimator::LM1027()
{
	return 1516.9891 * diff12 - 3822.9672 * diff13 + 122.4205 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 579.3711 * contr12 - 780.715 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 542.911 * contr34 + 2749.4114 ;
}

double DistanceEstimator::LM1028()
{
	return 1357.8033 * diff12 - 3822.9672 * diff13 + 122.4205 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 579.3711 * contr12 - 780.715 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 542.911 * contr34 + 2768.6863 ;
}

double DistanceEstimator::LM1029()
{
	return 4.7242 * diff12 - 4379.7162 * diff13 + 122.4205 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 579.3711 * contr12 - 797.1763 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 542.911 * contr34 + 3092.6047 ;
}

double DistanceEstimator::LM1030()
{
	return 4.7242 * diff12 - 4152.1981 * diff13 + 122.4205 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 579.3711 * contr12 - 730.9503 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 542.911 * contr34 + 3002.4987 ;
}

double DistanceEstimator::LM1031()
{
	return 4.7242 * diff12 - 4152.1981 * diff13 + 122.4205 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 579.3711 * contr12 - 740.061 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 542.911 * contr34 + 3005.6295 ;
}

double DistanceEstimator::LM1032()
{
	return 4.7242 * diff12 - 2393.2571 * diff13 + 122.4205 * diff14 + 1383.7932 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 579.3711 * contr12 - 1594.1776 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 718.8042 * contr34 + 2750.3686 ;
}

double DistanceEstimator::LM1033()
{
	return -843.7857 * diff12 - 88.2401 * diff13 + 122.4205 * diff14 + 2022.3247 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 817.9113 * contr12 - 4924.9093 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 1012.9424 * contr34 + 3035.2598 ;
}

double DistanceEstimator::LM1034()
{
	return -7500.7405 * diff12 - 88.2401 * diff13 + 122.4205 * diff14 + 2022.3247 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 2481.188 * contr12 - 5056.1462 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 1012.9424 * contr34 + 4229.6156 ;
}

double DistanceEstimator::LM1035()
{
	return -6937.1176 * diff12 - 88.2401 * diff13 + 122.4205 * diff14 + 2022.3247 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 2481.188 * contr12 - 5056.1462 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 1012.9424 * contr34 + 4169.7716 ;
}

double DistanceEstimator::LM1036()
{
	return -4266.1596 * diff12 - 88.2401 * diff13 + 122.4205 * diff14 + 2022.3247 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 - 2622.9352 * contr12 - 5056.1462 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 - 1012.9424 * contr34 + 3894.1685 ;
}

double DistanceEstimator::LM1037()
{
	return 1390.0332 * diff12 + 186.7917 * diff13 + 186.2311 * diff14 + 5075.5341 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 985.0402 * contr12 - 16.8547 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 49.1682 * contr34 + 448.929 ;
}

double DistanceEstimator::LM1038()
{
	return 1390.0332 * diff12 + 186.7917 * diff13 + 186.2311 * diff14 + 5075.5341 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 985.0402 * contr12 + 1815.761 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 49.1682 * contr34 - 113.9271 ;
}

double DistanceEstimator::LM1039()
{
	return 1390.0332 * diff12 + 186.7917 * diff13 + 186.2311 * diff14 + 5075.5341 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 985.0402 * contr12 + 2738.0051 * contr13 + 52.4361 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 49.1682 * contr34 - 388.7191 ;
}

double DistanceEstimator::LM1040()
{
	return -1666.1797 * diff12 + 186.7917 * diff13 + 186.2311 * diff14 + 2417.1806 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 1094.2862 * contr12 + 434.1593 * contr13 + 294.6333 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 49.1682 * contr34 + 1013.0243 ;
}

double DistanceEstimator::LM1041()
{
	return -1666.1797 * diff12 + 186.7917 * diff13 + 186.2311 * diff14 + 711.8152 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 1094.2862 * contr12 + 434.1593 * contr13 + 294.6333 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 49.1682 * contr34 + 1329.2823 ;
}

double DistanceEstimator::LM1042()
{
	return -1765.8471 * diff12 + 186.7917 * diff13 + 1188.9573 * diff14 + 8215.4232 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 1094.2862 * contr12 + 434.1593 * contr13 + 294.6333 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 49.1682 * contr34 - 315.1914 ;
}

double DistanceEstimator::LM1043()
{
	return 626.1712 * diff12 - 2534.5969 * diff13 + 186.2311 * diff14 + 7781.8347 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 1094.2862 * contr12 + 434.1593 * contr13 + 294.6333 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 49.1682 * contr34 + 519.8211 ;
}

double DistanceEstimator::LM1044()
{
	return 626.1712 * diff12 + 186.7917 * diff13 - 1137.6862 * diff14 + 4560.2573 * diff23 + 2.6997 * diff24 + 57.6698 * diff34 + 971.8945 * contr12 + 434.1593 * contr13 - 124.154 * contr14 - 2.3112 * contr23 + 2.0297 * contr24 + 49.1682 * contr34 + 779.3967 ;
}

double DistanceEstimator::LM1045()
{
	return -1227.2529 * diff12 - 643.1333 * diff13 + 2291.2229 * diff14 + 480.8614 * diff23 + 2.6997 * diff24 + 118.7477 * diff34 - 414.4852 * contr12 - 2.2419 * contr13 - 227.7628 * contr14 - 80.1744 * contr23 + 311.365 * contr24 + 64.8969 * contr34 + 1291.1184 ;
}

double DistanceEstimator::LM1046()
{
	return -1227.2529 * diff12 - 643.1333 * diff13 + 3125.964 * diff14 + 480.8614 * diff23 + 2.6997 * diff24 + 118.7477 * diff34 - 414.4852 * contr12 + 14.0893 * contr13 - 227.7628 * contr14 - 80.1744 * contr23 + 311.365 * contr24 + 64.8969 * contr34 + 1057.7311 ;
}

double DistanceEstimator::LM1047()
{
	return -1227.2529 * diff12 - 643.1333 * diff13 + 547.6301 * diff14 + 480.8614 * diff23 + 2.6997 * diff24 + 118.7477 * diff34 - 414.4852 * contr12 + 46.7515 * contr13 - 227.7628 * contr14 - 80.1744 * contr23 + 311.365 * contr24 + 64.8969 * contr34 + 1743.6078 ;
}

double DistanceEstimator::LM1048()
{
	return -4719.4088 * diff12 - 643.1333 * diff13 - 1201.4335 * diff14 + 480.8614 * diff23 + 2.6997 * diff24 + 118.7477 * diff34 - 1029.1799 * contr12 + 46.7515 * contr13 - 95.153 * contr14 + 502.7367 * contr23 + 225.0405 * contr24 + 64.8969 * contr34 + 2592.7891 ;
}

double DistanceEstimator::LM1049()
{
	return -2075.3902 * diff12 - 643.1333 * diff13 - 76.8928 * diff14 + 480.8614 * diff23 + 2.6997 * diff24 + 118.7477 * diff34 - 307.3365 * contr12 + 46.7515 * contr13 - 344.0191 * contr14 - 80.1744 * contr23 + 225.0405 * contr24 + 64.8969 * contr34 + 2038.78 ;
}

double DistanceEstimator::LM1050()
{
	return -682.7711 * diff12 - 737.3825 * diff13 - 4.6684 * diff14 + 589.1488 * diff23 + 2.6997 * diff24 + 118.7477 * diff34 - 277.1633 * contr12 + 46.7515 * contr13 - 86.7304 * contr14 - 1961.3061 * contr23 + 141.415 * contr24 + 64.8969 * contr34 + 1881.3945 ;
}

double DistanceEstimator::LM1051()
{
	return -682.7711 * diff12 - 737.3825 * diff13 - 4.6684 * diff14 + 589.1488 * diff23 + 2.6997 * diff24 + 118.7477 * diff34 - 277.1633 * contr12 + 46.7515 * contr13 - 86.7304 * contr14 - 80.1744 * contr23 + 141.415 * contr24 + 64.8969 * contr34 + 1844.8219 ;
}

double DistanceEstimator::LM1052()
{
	return -1052.9853 * diff12 - 737.3825 * diff13 - 4.6684 * diff14 + 589.1488 * diff23 + 2.6997 * diff24 + 118.7477 * diff34 - 370.7364 * contr12 + 46.7515 * contr13 - 86.7304 * contr14 - 80.1744 * contr23 + 141.415 * contr24 + 64.8969 * contr34 + 1876.1361 ;
}

double DistanceEstimator::LM1053()
{
	return -2298.0124 * diff12 - 737.3825 * diff13 - 4.6684 * diff14 + 589.1488 * diff23 + 2.6997 * diff24 + 118.7477 * diff34 - 370.7364 * contr12 + 46.7515 * contr13 - 86.7304 * contr14 - 587.2216 * contr23 + 141.415 * contr24 + 64.8969 * contr34 + 2003.2724 ;
}

double DistanceEstimator::LM1054()
{
	return -2829.6676 * diff12 - 737.3825 * diff13 - 4.6684 * diff14 + 589.1488 * diff23 + 2.6997 * diff24 + 118.7477 * diff34 - 370.7364 * contr12 + 46.7515 * contr13 - 86.7304 * contr14 - 429.8621 * contr23 + 141.415 * contr24 + 64.8969 * contr34 + 2040.0706 ;
}

double DistanceEstimator::LM1055()
{
	return -649.4379 * diff12 - 655.5862 * diff13 - 4.6684 * diff14 - 14.8631 * diff23 + 2.6997 * diff24 + 126.5216 * diff34 + 362.0785 * contr12 - 459.367 * contr13 + 18.9324 * contr14 - 161.4061 * contr23 + 166.1321 * contr24 + 64.8969 * contr34 + 1842.1479 ;
}

double DistanceEstimator::LM1056()
{
	return -649.4379 * diff12 - 1269.5955 * diff13 - 908.2829 * diff14 - 14.8631 * diff23 + 2.6997 * diff24 + 126.5216 * diff34 + 596.3529 * contr12 - 341.5038 * contr13 + 18.9324 * contr14 - 161.4061 * contr23 + 562.5288 * contr24 + 64.8969 * contr34 + 2071.9933 ;
}

double DistanceEstimator::LM1057()
{
	return -649.4379 * diff12 - 1095.3298 * diff13 - 908.2829 * diff14 - 14.8631 * diff23 + 2.6997 * diff24 + 126.5216 * diff34 + 596.3529 * contr12 - 341.5038 * contr13 + 18.9324 * contr14 - 161.4061 * contr23 + 562.5288 * contr24 + 64.8969 * contr34 + 2023.3725 ;
}

double DistanceEstimator::LM1058()
{
	return -649.4379 * diff12 - 1862.099 * diff13 - 1001.2025 * diff14 - 14.8631 * diff23 + 2.6997 * diff24 + 126.5216 * diff34 + 596.3529 * contr12 - 341.5038 * contr13 + 18.9324 * contr14 - 161.4061 * contr23 + 562.5288 * contr24 + 64.8969 * contr34 + 2260.0749 ;
}

double DistanceEstimator::LM1059()
{
	return -649.4379 * diff12 - 1580.4038 * diff13 - 173.4533 * diff14 - 14.8631 * diff23 + 2.6997 * diff24 + 126.5216 * diff34 + 596.3529 * contr12 - 341.5038 * contr13 + 18.9324 * contr14 - 161.4061 * contr23 + 968.7285 * contr24 + 64.8969 * contr34 + 1883.502 ;
}

double DistanceEstimator::LM1060()
{
	return -649.4379 * diff12 - 2557.5524 * diff13 - 4.6684 * diff14 - 1031.248 * diff23 + 2.6997 * diff24 + 126.5216 * diff34 + 976.3007 * contr12 - 341.5038 * contr13 + 18.9324 * contr14 - 161.4061 * contr23 + 568.8495 * contr24 + 64.8969 * contr34 + 2356.4903 ;
}

double DistanceEstimator::LM1061()
{
	return -649.4379 * diff12 - 2511.4079 * diff13 - 4.6684 * diff14 - 1248.6468 * diff23 + 2.6997 * diff24 + 126.5216 * diff34 + 976.3007 * contr12 - 341.5038 * contr13 + 18.9324 * contr14 - 161.4061 * contr23 + 568.8495 * contr24 + 64.8969 * contr34 + 2379.9629 ;
}

double DistanceEstimator::LM1062()
{
	return -649.4379 * diff12 - 2511.4079 * diff13 - 4.6684 * diff14 - 1295.1642 * diff23 + 2.6997 * diff24 + 126.5216 * diff34 + 976.3007 * contr12 - 341.5038 * contr13 + 18.9324 * contr14 - 161.4061 * contr23 + 568.8495 * contr24 + 64.8969 * contr34 + 2388.0549 ;
}

double DistanceEstimator::LM1063()
{
	return -1164.4167 * diff12 - 233.6367 * diff13 - 4.6684 * diff14 + 2143.213 * diff23 + 2.6997 * diff24 - 930.0346 * diff34 - 173.924 * contr12 + 46.7515 * contr13 + 18.9324 * contr14 - 616.855 * contr23 + 142.6889 * contr24 + 64.8969 * contr34 + 1490.2725 ;
}

double DistanceEstimator::LM1064()
{
	return -1097.2197 * diff12 - 233.6367 * diff13 - 4.6684 * diff14 + 410.459 * diff23 + 2.6997 * diff24 + 126.5216 * diff34 - 103.3555 * contr12 + 46.7515 * contr13 + 18.9324 * contr14 - 329.7715 * contr23 - 964.4352 * contr24 + 64.8969 * contr34 + 1977.3744 ;
}

double DistanceEstimator::LM1065()
{
	return -1097.2197 * diff12 - 233.6367 * diff13 - 4.6684 * diff14 + 410.459 * diff23 + 2.6997 * diff24 + 126.5216 * diff34 - 103.3555 * contr12 + 46.7515 * contr13 + 18.9324 * contr14 - 329.7715 * contr23 - 930.4076 * contr24 + 64.8969 * contr34 + 1963.8399 ;
}

double DistanceEstimator::LM1066()
{
	return -1097.2197 * diff12 - 233.6367 * diff13 - 4.6684 * diff14 + 179.1434 * diff23 + 2.6997 * diff24 + 126.5216 * diff34 - 103.3555 * contr12 + 46.7515 * contr13 + 18.9324 * contr14 - 329.7715 * contr23 - 36.367 * contr24 + 64.8969 * contr34 + 1796.3219 ;
}

double DistanceEstimator::LM1067()
{
	return -1097.2197 * diff12 - 233.6367 * diff13 - 4.6684 * diff14 + 179.1434 * diff23 + 2.6997 * diff24 + 126.5216 * diff34 + 59.2216 * contr12 + 46.7515 * contr13 + 18.9324 * contr14 - 329.7715 * contr23 - 36.367 * contr24 + 64.8969 * contr34 + 1758.4628 ;
}

double DistanceEstimator::LM1068()
{
	return -1097.2197 * diff12 - 233.6367 * diff13 - 4.6684 * diff14 + 179.1434 * diff23 + 2.6997 * diff24 + 126.5216 * diff34 + 110.5617 * contr12 + 46.7515 * contr13 + 18.9324 * contr14 - 329.7715 * contr23 - 36.367 * contr24 + 64.8969 * contr34 + 1751.0334 ;
}

double DistanceEstimator::LM1069()
{
	return -1167.1393 * diff12 - 233.6367 * diff13 - 4.6684 * diff14 - 14.8631 * diff23 + 2.6997 * diff24 + 126.5216 * diff34 - 103.3555 * contr12 + 46.7515 * contr13 + 18.9324 * contr14 - 329.7715 * contr23 + 142.6889 * contr24 + 64.8969 * contr34 + 1783.0558 ;
}

double DistanceEstimator::LM1070()
{
	return -953.4617 * diff12 - 1563.7065 * diff13 - 4.6684 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 493.6031 * diff34 - 342.0644 * contr12 + 119.2145 * contr13 + 67.339 * contr14 + 6.5852 * contr23 + 2.0297 * contr24 + 120.3375 * contr34 + 2292.7973 ;
}

double DistanceEstimator::LM1071()
{
	return -953.4617 * diff12 - 1563.7065 * diff13 - 4.6684 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 493.6031 * diff34 - 372.1179 * contr12 + 119.2145 * contr13 + 67.339 * contr14 + 6.5852 * contr23 + 2.0297 * contr24 + 120.3375 * contr34 + 2280.2871 ;
}

double DistanceEstimator::LM1072()
{
	return -742.6383 * diff12 - 2038.7552 * diff13 - 4.6684 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 819.7008 * diff34 - 112.7175 * contr12 + 119.2145 * contr13 + 118.5789 * contr14 + 156.603 * contr23 + 2.0297 * contr24 + 120.3375 * contr34 + 2382.6839 ;
}

double DistanceEstimator::LM1073()
{
	return -742.6383 * diff12 - 2304.3552 * diff13 - 4.6684 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 931.6447 * diff34 - 1059.2889 * contr12 + 119.2145 * contr13 + 118.5789 * contr14 + 887.3842 * contr23 + 2.0297 * contr24 + 120.3375 * contr34 + 2540.3418 ;
}

double DistanceEstimator::LM1074()
{
	return -742.6383 * diff12 - 2304.3552 * diff13 - 4.6684 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 1324.8448 * diff34 - 255.0658 * contr12 + 119.2145 * contr13 + 118.5789 * contr14 + 584.1291 * contr23 + 2.0297 * contr24 + 120.3375 * contr34 + 2455.8808 ;
}

double DistanceEstimator::LM1075()
{
	return -742.6383 * diff12 - 2304.3552 * diff13 - 4.6684 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 1493.3591 * diff34 - 255.0658 * contr12 + 119.2145 * contr13 + 118.5789 * contr14 + 584.1291 * contr23 + 2.0297 * contr24 + 120.3375 * contr34 + 2457.7467 ;
}

double DistanceEstimator::LM1076()
{
	return -4293.3895 * diff12 - 680.1827 * diff13 - 4.6684 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 493.6031 * diff34 + 504.7834 * contr12 + 856.4276 * contr13 + 177.2859 * contr14 - 76.0957 * contr23 + 2.0297 * contr24 + 120.3375 * contr34 + 1941.2007 ;
}

double DistanceEstimator::LM1077()
{
	return -1345.1982 * diff12 - 4645.9456 * diff13 - 4.6684 * diff14 + 600.6459 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 122.0785 * contr12 + 204.0763 * contr13 + 81.385 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 2993.8552 ;
}

double DistanceEstimator::LM1078()
{
	return -1345.1982 * diff12 - 4645.9456 * diff13 - 4.6684 * diff14 + 600.6459 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 122.0785 * contr12 + 204.0763 * contr13 + 81.385 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 2996.8827 ;
}

double DistanceEstimator::LM1079()
{
	return -1345.1982 * diff12 - 4645.9456 * diff13 - 4.6684 * diff14 + 600.6459 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 122.0785 * contr12 + 204.0763 * contr13 + 81.385 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 2986.8833 ;
}

double DistanceEstimator::LM1080()
{
	return 4.7242 * diff12 - 6668.5446 * diff13 - 4.6684 * diff14 + 600.6459 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 122.0785 * contr12 + 204.0763 * contr13 + 81.385 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 3434.6172 ;
}

double DistanceEstimator::LM1081()
{
	return -3006.5371 * diff12 - 3305.9118 * diff13 - 4.6684 * diff14 + 600.6459 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 122.0785 * contr12 + 204.0763 * contr13 + 81.385 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 2708.1051 ;
}

double DistanceEstimator::LM1082()
{
	return 4.7242 * diff12 + 3960.078 * diff13 - 4.6684 * diff14 + 2948.8778 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 795.1771 * contr12 + 132.8311 * contr13 + 25.7928 * contr14 + 939.5141 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 148.0047 ;
}

double DistanceEstimator::LM1083()
{
	return 4.7242 * diff12 + 896.9672 * diff13 - 4.6684 * diff14 + 761.5244 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 1411.9246 * contr12 - 44.1467 * contr13 - 574.3375 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 1644.5609 ;
}

double DistanceEstimator::LM1084()
{
	return 4.7242 * diff12 + 896.9672 * diff13 - 4.6684 * diff14 + 761.5244 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 1184.9345 * contr12 - 44.1467 * contr13 - 616.4768 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 1603.1232 ;
}

double DistanceEstimator::LM1085()
{
	return 4.7242 * diff12 + 896.9672 * diff13 - 4.6684 * diff14 + 761.5244 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 1184.9345 * contr12 - 44.1467 * contr13 - 631.8805 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 1595.8372 ;
}

double DistanceEstimator::LM1086()
{
	return 4.7242 * diff12 + 896.9672 * diff13 - 4.6684 * diff14 + 761.5244 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 1151.7276 * contr12 - 44.1467 * contr13 - 580.9063 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 1572.7981 ;
}

double DistanceEstimator::LM1087()
{
	return -1239.0928 * diff12 - 1505.7307 * diff13 - 4.6684 * diff14 + 761.5244 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 99.7943 * contr12 + 297.9382 * contr13 + 81.385 * contr14 + 493.2068 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 2055.6997 ;
}

double DistanceEstimator::LM1088()
{
	return -2216.6791 * diff12 - 1505.7307 * diff13 - 4.6684 * diff14 + 761.5244 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 99.7943 * contr12 + 297.9382 * contr13 + 81.385 * contr14 + 468.4622 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 2165.1096 ;
}

double DistanceEstimator::LM1089()
{
	return -1713.8081 * diff12 - 1505.7307 * diff13 - 4.6684 * diff14 + 761.5244 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 99.7943 * contr12 + 297.9382 * contr13 + 81.385 * contr14 + 629.083 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 2085.7868 ;
}

double DistanceEstimator::LM1090()
{
	return 1456.3082 * diff12 - 9608.4181 * diff13 - 4.6684 * diff14 + 5116.8888 * diff23 + 2.6997 * diff24 - 967.5652 * diff34 - 402.7032 * contr12 + 554.0063 * contr13 - 108.9242 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 3407.9157 ;
}

double DistanceEstimator::LM1091()
{
	return 1456.3082 * diff12 - 9772.9681 * diff13 - 4.6684 * diff14 + 5116.8888 * diff23 + 2.6997 * diff24 - 967.5652 * diff34 - 402.7032 * contr12 + 554.0063 * contr13 - 108.9242 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 3455.1006 ;
}

double DistanceEstimator::LM1092()
{
	return 1456.3082 * diff12 - 9004.0614 * diff13 - 4.6684 * diff14 + 5116.8888 * diff23 + 2.6997 * diff24 - 967.5652 * diff34 - 402.7032 * contr12 + 554.0063 * contr13 - 82.1022 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 3232.4846 ;
}

double DistanceEstimator::LM1093()
{
	return 1456.3082 * diff12 - 7737.3817 * diff13 + 7031.9135 * diff14 + 5116.8888 * diff23 + 2.6997 * diff24 - 1778.6911 * diff34 - 402.7032 * contr12 + 554.0063 * contr13 - 326.1692 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 1408.636 ;
}

double DistanceEstimator::LM1094()
{
	return 2572.9112 * diff12 - 12376.1296 * diff13 - 4.6684 * diff14 + 5116.8888 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 402.7032 * contr12 + 554.0063 * contr13 + 1039.4089 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 3976.5265 ;
}

double DistanceEstimator::LM1095()
{
	return 2572.9112 * diff12 - 12376.1296 * diff13 - 4.6684 * diff14 + 5116.8888 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 402.7032 * contr12 + 554.0063 * contr13 - 589.2328 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 4000.7618 ;
}

double DistanceEstimator::LM1096()
{
	return -2806.4712 * diff12 - 1707.0094 * diff13 - 8380.9184 * diff14 + 9852.3408 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 1018.0673 * contr12 + 491.2912 * contr13 - 193.4095 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 2407.1577 ;
}

double DistanceEstimator::LM1097()
{
	return -11382.89 * diff12 - 1707.0094 * diff13 - 12316.1921 * diff14 + 9852.3408 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 1018.0673 * contr12 + 491.2912 * contr13 - 193.4095 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 3980.9813 ;
}

double DistanceEstimator::LM1098()
{
	return -11000.1288 * diff12 - 1707.0094 * diff13 - 12316.1921 * diff14 + 9852.3408 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 1018.0673 * contr12 + 491.2912 * contr13 - 193.4095 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 + 3941.8318 ;
}

double DistanceEstimator::LM1099()
{
	return 4.7242 * diff12 - 1707.0094 * diff13 - 1405.6276 * diff14 + 28140.6936 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 864.6341 * contr12 + 491.2912 * contr13 - 130.4358 * contr14 + 6.5852 * contr23 - 25.6227 * contr24 + 95.5129 * contr34 - 3145.4519 ;
}

double DistanceEstimator::LM1100()
{
	return 4.7242 * diff12 + 204.9614 * diff13 - 4.6684 * diff14 + 4149.482 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 + 254.6162 * contr12 + 66.3244 * contr13 + 130.7451 * contr14 + 6.5852 * contr23 - 215.6894 * contr24 + 145.641 * contr34 + 911.0481 ;
}

double DistanceEstimator::LM1101()
{
	return 4.7242 * diff12 + 204.9614 * diff13 - 4.6684 * diff14 + 4149.482 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 + 254.6162 * contr12 + 66.3244 * contr13 + 130.7451 * contr14 + 6.5852 * contr23 - 241.9074 * contr24 + 145.641 * contr34 + 912.8658 ;
}

double DistanceEstimator::LM1102()
{
	return 4.7242 * diff12 + 204.9614 * diff13 - 4.6684 * diff14 + 3320.421 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 + 254.6162 * contr12 + 66.3244 * contr13 + 130.7451 * contr14 + 6.5852 * contr23 - 218.5564 * contr24 + 145.641 * contr34 + 1100.5773 ;
}

double DistanceEstimator::LM1103()
{
	return 4.7242 * diff12 + 204.9614 * diff13 - 4.6684 * diff14 + 3320.421 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 + 254.6162 * contr12 + 66.3244 * contr13 + 130.7451 * contr14 + 6.5852 * contr23 - 233.5514 * contr24 + 145.641 * contr34 + 1094.6704 ;
}

double DistanceEstimator::LM1104()
{
	return 2294.0289 * diff12 + 92.941 * diff13 - 2263.141 * diff14 + 3115.6988 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 + 4535.3878 * contr12 + 66.3244 * contr13 + 1098.5602 * contr14 + 6.5852 * contr23 + 233.2886 * contr24 + 145.641 * contr34 + 839.6502 ;
}

double DistanceEstimator::LM1105()
{
	return 2294.0289 * diff12 + 92.941 * diff13 - 2263.141 * diff14 + 3115.6988 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 + 4535.3878 * contr12 + 66.3244 * contr13 + 1098.5602 * contr14 + 6.5852 * contr23 + 233.2886 * contr24 + 145.641 * contr34 + 839.3011 ;
}

double DistanceEstimator::LM1106()
{
	return 2294.0289 * diff12 + 92.941 * diff13 - 2415.8664 * diff14 + 3115.6988 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 + 4535.3878 * contr12 + 66.3244 * contr13 + 1098.5602 * contr14 + 6.5852 * contr23 + 233.2886 * contr24 + 145.641 * contr34 + 873.5817 ;
}

double DistanceEstimator::LM1107()
{
	return 2896.4775 * diff12 + 92.941 * diff13 - 1743.8744 * diff14 + 3115.6988 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 + 5191.0318 * contr12 + 66.3244 * contr13 + 1098.5602 * contr14 + 6.5852 * contr23 + 233.2886 * contr24 + 145.641 * contr34 + 647.021 ;
}

double DistanceEstimator::LM1108()
{
	return 4.7242 * diff12 + 2018.0282 * diff13 - 765.571 * diff14 + 4066.6388 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 712.5985 * contr12 + 66.3244 * contr13 + 921.0885 * contr14 + 6.5852 * contr23 + 233.2886 * contr24 + 145.641 * contr34 + 495.3629 ;
}

double DistanceEstimator::LM1109()
{
	return 4.7242 * diff12 + 2018.0282 * diff13 - 765.571 * diff14 + 4066.6388 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 - 712.5985 * contr12 + 66.3244 * contr13 + 921.0885 * contr14 + 6.5852 * contr23 + 233.2886 * contr24 + 145.641 * contr34 + 492.9083 ;
}

double DistanceEstimator::LM1110()
{
	return 4.7242 * diff12 + 2018.0282 * diff13 - 765.571 * diff14 + 4066.6388 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 + 1335.5593 * contr12 + 66.3244 * contr13 + 921.0885 * contr14 + 6.5852 * contr23 + 233.2886 * contr24 + 145.641 * contr34 + 269.0448 ;
}

double DistanceEstimator::LM1111()
{
	return 4.7242 * diff12 + 2278.1751 * diff13 - 765.571 * diff14 + 4734.7642 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 + 1543.0123 * contr12 + 66.3244 * contr13 + 970.6728 * contr14 + 6.5852 * contr23 + 233.2886 * contr24 + 145.641 * contr34 + 47.0157 ;
}

double DistanceEstimator::LM1112()
{
	return 4.7242 * diff12 + 2278.1751 * diff13 - 765.571 * diff14 + 4988.456 * diff23 + 2.6997 * diff24 + 276.1613 * diff34 + 1543.0123 * contr12 + 66.3244 * contr13 + 970.6728 * contr14 + 6.5852 * contr23 + 233.2886 * contr24 + 145.641 * contr34 - 0.2651 ;
}

double DistanceEstimator::LM1113()
{
	return 1064.1482 * diff12 + 472.648 * diff13 - 4.6684 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 1314.6666 * contr12 - 515.7858 * contr13 - 8.5009 * contr14 - 31.417 * contr23 + 31.8114 * contr24 + 55.9543 * contr34 + 2072.2546 ;
}

double DistanceEstimator::LM1114()
{
	return 3074.1578 * diff12 + 472.648 * diff13 - 4.6684 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 896.683 * contr12 - 2791.5796 * contr13 - 8.5009 * contr14 - 31.417 * contr23 + 31.8114 * contr24 + 55.9543 * contr34 + 2472.1966 ;
}

double DistanceEstimator::LM1115()
{
	return 4.7242 * diff12 + 2767.9778 * diff13 - 4.6684 * diff14 + 1118.9861 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 + 223.7937 * contr12 - 617.6112 * contr13 + 302.8515 * contr14 - 31.417 * contr23 + 31.8114 * contr24 + 55.9543 * contr34 + 1011.5457 ;
}

double DistanceEstimator::LM1116()
{
	return 1705.2655 * diff12 + 310.1245 * diff13 - 4.6684 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 4284.0256 * contr12 + 309.109 * contr13 + 224.3373 * contr14 - 31.417 * contr23 + 31.8114 * contr24 + 55.9543 * contr34 + 2207.3632 ;
}

double DistanceEstimator::LM1117()
{
	return 1705.2655 * diff12 + 310.1245 * diff13 - 4.6684 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 4126.5391 * contr12 + 309.109 * contr13 + 224.3373 * contr14 - 31.417 * contr23 + 31.8114 * contr24 + 55.9543 * contr34 + 2163.3766 ;
}

double DistanceEstimator::LM1118()
{
	return 2258.5907 * diff12 + 310.1245 * diff13 - 4.6684 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 4126.5391 * contr12 + 309.109 * contr13 + 224.3373 * contr14 - 31.417 * contr23 + 31.8114 * contr24 + 55.9543 * contr34 + 2111.799 ;
}

double DistanceEstimator::LM1119()
{
	return 1487.2473 * diff12 + 310.1245 * diff13 - 4.6684 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 2572.98 * contr12 + 3366.2565 * contr13 + 194.4862 * contr14 - 31.417 * contr23 + 31.8114 * contr24 + 55.9543 * contr34 + 1038.8412 ;
}

double DistanceEstimator::LM1120()
{
	return -7394.4554 * diff12 - 102.0636 * diff13 - 2077.9049 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 236.0791 * contr12 + 866.2109 * contr13 - 8.5009 * contr14 - 31.417 * contr23 + 168.3465 * contr24 + 211.584 * contr34 + 2491.6176 ;
}

double DistanceEstimator::LM1121()
{
	return -5490.1079 * diff12 - 102.0636 * diff13 - 2077.9049 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 236.0791 * contr12 + 866.2109 * contr13 - 8.5009 * contr14 - 31.417 * contr23 + 168.3465 * contr24 + 211.584 * contr34 + 2374.3772 ;
}

double DistanceEstimator::LM1122()
{
	return -5215.3094 * diff12 + 1266.4574 * diff13 - 2152.7368 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 236.0791 * contr12 + 866.2109 * contr13 - 8.5009 * contr14 - 31.417 * contr23 + 168.3465 * contr24 + 220.044 * contr34 + 2047.3816 ;
}

double DistanceEstimator::LM1123()
{
	return -5215.3094 * diff12 - 555.7349 * diff13 - 2152.7368 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 236.0791 * contr12 + 866.2109 * contr13 - 8.5009 * contr14 - 31.417 * contr23 + 168.3465 * contr24 + 220.044 * contr34 + 2534.0373 ;
}

double DistanceEstimator::LM1124()
{
	return -2589.7773 * diff12 - 102.0636 * diff13 - 2052.0652 * diff14 + 25.4087 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 236.0791 * contr12 + 866.2109 * contr13 - 8.5009 * contr14 - 31.417 * contr23 + 168.3465 * contr24 + 112.8837 * contr34 + 2296.5999 ;
}

double DistanceEstimator::LM1125()
{
	return 478.4736 * diff12 + 509.2296 * diff13 - 222.8115 * diff14 - 131.535 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 79.2539 * contr12 + 19.2655 * contr13 + 6.1367 * contr14 + 156.5405 * contr23 - 16.645 * contr24 + 26.3356 * contr34 + 1763.6877 ;
}

double DistanceEstimator::LM1126()
{
	return 478.4736 * diff12 + 595.5863 * diff13 - 222.8115 * diff14 - 131.535 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 79.2539 * contr12 + 19.2655 * contr13 + 6.1367 * contr14 + 156.5405 * contr23 - 16.645 * contr24 + 26.3356 * contr34 + 1756.1412 ;
}

double DistanceEstimator::LM1127()
{
	return 3563.6968 * diff12 - 181.6241 * diff13 - 860.2825 * diff14 - 131.535 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 187.6009 * contr12 + 19.2655 * contr13 + 6.1367 * contr14 + 872.2561 * contr23 - 16.645 * contr24 + 26.3356 * contr34 + 1916.0654 ;
}

double DistanceEstimator::LM1128()
{
	return 403.9144 * diff12 - 181.6241 * diff13 - 289.5467 * diff14 - 356.9121 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 + 10.8951 * contr12 + 19.2655 * contr13 + 6.1367 * contr14 + 17.8116 * contr23 - 16.645 * contr24 + 26.3356 * contr34 + 2075.0947 ;
}

double DistanceEstimator::LM1129()
{
	return 721.2936 * diff12 - 181.6241 * diff13 - 263.0353 * diff14 - 356.9121 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 + 3.9584 * contr12 + 19.2655 * contr13 + 6.1367 * contr14 + 268.2107 * contr23 - 16.645 * contr24 + 26.3356 * contr34 + 2038.9409 ;
}

double DistanceEstimator::LM1130()
{
	return 463.3167 * diff12 - 181.6241 * diff13 - 371.1195 * diff14 - 356.9121 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 + 70.7171 * contr12 - 22.4272 * contr13 + 6.1367 * contr14 + 3.0949 * contr23 - 16.645 * contr24 + 26.3356 * contr34 + 2116.9959 ;
}

double DistanceEstimator::LM1131()
{
	return 463.3167 * diff12 - 181.6241 * diff13 - 126.285 * diff14 - 356.9121 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 241.7937 * contr12 - 260.9393 * contr13 + 6.1367 * contr14 + 3.0949 * contr23 - 16.645 * contr24 + 26.3356 * contr34 + 2174.8449 ;
}

double DistanceEstimator::LM1132()
{
	return 463.3167 * diff12 - 181.6241 * diff13 - 191.1413 * diff14 - 356.9121 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 - 50.3799 * contr12 - 260.9393 * contr13 + 6.1367 * contr14 + 3.0949 * contr23 - 16.645 * contr24 + 26.3356 * contr34 + 2165.4558 ;
}

double DistanceEstimator::LM1133()
{
	return 463.3167 * diff12 - 181.6241 * diff13 - 413.5055 * diff14 + 807.2843 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 + 96.8969 * contr12 - 380.565 * contr13 + 6.1367 * contr14 + 3.0949 * contr23 - 16.645 * contr24 + 26.3356 * contr34 + 1956.9171 ;
}

double DistanceEstimator::LM1134()
{
	return 4.7242 * diff12 + 183.8712 * diff13 - 284.1706 * diff14 - 705.4201 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 + 1229.5623 * contr12 + 109.339 * contr13 + 6.1367 * contr14 - 162.7875 * contr23 - 16.645 * contr24 + 26.3356 * contr34 + 1992.3887 ;
}

double DistanceEstimator::LM1135()
{
	return 4.7242 * diff12 + 183.8712 * diff13 - 284.1706 * diff14 - 297.5019 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 + 208.8251 * contr12 + 52.8035 * contr13 + 6.1367 * contr14 - 162.7875 * contr23 - 16.645 * contr24 + 26.3356 * contr34 + 2022.899 ;
}

double DistanceEstimator::LM1136()
{
	return 4.7242 * diff12 + 2220.7788 * diff13 - 284.1706 * diff14 + 727.466 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 + 1627.8809 * contr12 + 19.2655 * contr13 + 6.1367 * contr14 - 412.1018 * contr23 - 16.645 * contr24 + 26.3356 * contr34 + 998.5956 ;
}

double DistanceEstimator::LM1137()
{
	return 4.7242 * diff12 + 4904.275 * diff13 - 284.1706 * diff14 + 727.466 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 + 1586.3911 * contr12 - 306.7943 * contr13 + 6.1367 * contr14 - 412.1018 * contr23 - 16.645 * contr24 + 26.3356 * contr34 + 346.6338 ;
}

double DistanceEstimator::LM1138()
{
	return 4.7242 * diff12 + 5419.9341 * diff13 - 284.1706 * diff14 + 727.466 * diff23 + 2.6997 * diff24 + 86.2354 * diff34 + 1586.3911 * contr12 - 264.2648 * contr13 + 6.1367 * contr14 - 412.1018 * contr23 - 16.645 * contr24 + 26.3356 * contr34 + 178.3461 ;
}
