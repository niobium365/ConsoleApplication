README

0 cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=/bin/clang-11 -DCMAKE_CXX_COMPILER:FILEPATH=/bin/clang++-11 -Bbuild -G Ninja

1 本测试未参考其他资料.

2 测试工程在visual studio 2019 + (MSVC2019 或 clang10-cl) 32 位模式下编译通过 完成. 如需要x64 还需要对size_t进行处理

3 json::any 直接使用了std::any 并对 const char*做了特殊处理.

4 由于以前从没用过RapidJason库 (我自己一直使用nlohmann::json) 所以有些RapidJason 的API使用起来可能比较笨拙 :)

5 要说句抱歉这个测试使用了C++17 标准, 主要是inline variable. 转成C++14可能要稍微做点工作.

6 如果用std::tuple<...> 来描述 Address, Firend, Address等实体, 序列化/反序列化 皆可用通用泛型来完成.

7 序列化结果

{
    "tag_data": {
        "name": {
            "tag_data": "p1",
            "tag_type": 1604120214
        },
        "age": {
            "tag_data": 4,
            "tag_type": 3440116983
        },
        "address": {
            "tag_data": {
                "country": {
                    "tag_data": "china",
                    "tag_type": 1604120214
                },
                "city": {
                    "tag_data": "beijing",
                    "tag_type": 1604120214
                },
                "street": {
                    "tag_data": "wangjing",
                    "tag_type": 1604120214
                },
                "neighbors": {
                    "tag_data": [
                        {
                            "tag_data": {
                                "name": {
                                    "tag_data": "p2",
                                    "tag_type": 1604120214
                                },
                                "age": {
                                    "tag_data": 3,
                                    "tag_type": 3440116983
                                },
                                "address": {
                                    "tag_data": {
                                        "country": {
                                            "tag_data": "china",
                                            "tag_type": 1604120214
                                        },
                                        "city": {
                                            "tag_data": "shanghai",
                                            "tag_type": 1604120214
                                        },
                                        "street": {
                                            "tag_data": "putuo",
                                            "tag_type": 1604120214
                                        },
                                        "neighbors": {
                                            "tag_data": [],
                                            "tag_type": 918223931
                                        }
                                    },
                                    "tag_type": 2979733288
                                },
                                "_friends": {
                                    "tag_data": [],
                                    "tag_type": 3435942975
                                },
                                "secret": {
                                    "tag_data": {
                                        "any_data": null
                                    },
                                    "tag_type": 1717394816
                                }
                            },
                            "tag_type": 4026859749
                        }
                    ],
                    "tag_type": 918223931
                }
            },
            "tag_type": 2979733288
        },
        "_friends": {
            "tag_data": [
                {
                    "tag_data": {
                        "relation": {
                            "tag_data": "my best friend",
                            "tag_type": 1604120214
                        },
                        "secret": {
                            "tag_data": {
                                "any_data": {
                                    "tag_data": {
                                        "type": {
                                            "tag_data": "rocker",
                                            "tag_type": 1604120214
                                        },
                                        "age": {
                                            "tag_data": 18,
                                            "tag_type": 3440116983
                                        }
                                    },
                                    "tag_type": 2938238180
                                }
                            },
                            "tag_type": 1717394816
                        }
                    },
                    "tag_type": 2301425872
                },
                {
                    "tag_data": {
                        "relation": {
                            "tag_data": "new friend",
                            "tag_type": 1604120214
                        },
                        "secret": {
                            "tag_data": {
                                "any_data": {
                                    "tag_data": "little girl",
                                    "tag_type": 1604120214
                                }
                            },
                            "tag_type": 1717394816
                        }
                    },
                    "tag_type": 2301425872
                },
                {
                    "tag_data": {
                        "relation": {
                            "tag_data": "third friend",
                            "tag_type": 1604120214
                        },
                        "secret": {
                            "tag_data": {
                                "any_data": {
                                    "tag_data": 3,
                                    "tag_type": 3440116983
                                }
                            },
                            "tag_type": 1717394816
                        }
                    },
                    "tag_type": 2301425872
                }
            ],
            "tag_type": 3435942975
        },
        "secret": {
            "tag_data": {
                "any_data": {
                    "tag_data": "the kind!",
                    "tag_type": 1604120214
                }
            },
            "tag_type": 1717394816
        }
    },
    "tag_type": 4026859749
}

7 谢谢, 希望有时间多交流.


杜景波


