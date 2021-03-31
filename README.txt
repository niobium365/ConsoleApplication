README

0 cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=/bin/clang-11 -DCMAKE_CXX_COMPILER:FILEPATH=/bin/clang++-11 -Bbuild -G Ninja

1 本测试程序新版参考了 https://www.boost.org/doc/libs/1_75_0/libs/serialization/doc/index.html.

2 测试工程在CMAKE clang11 Ubuntu20 x64模式下编译通过 完成.

3 第二版 已经完成序列化和 反序列化部分, 剩余工作包括 
3.0 去除对boost的依赖
3.1 反序列化失败则抛异常, 
3.2 反序列化 适配通用STL情景 
3.3 支持所有RFC 标准的json 所有类型 比如binary 等
3.4 优化(比如 尽可能减少对象拷贝).

4 本测试使用RapidJason库, 还可以进一步试用nlohmann::json

5 这个测试使用了C++14 标准.

6 新版中反序列化 应该分2层, 一层适配通用STL情景, 一层在boost::serialization 名字空间下用户定制.

7 新版序列化结果

{
    "name": "p1",
    "age": 4,
    "address": {
        "country": "china",
        "city": "beijing",
        "street": "wangjing",
        "neighbors": [
            {
                "name": "p2",
                "age": 3,
                "address": {
                    "country": "china",
                    "city": "shanghai",
                    "street": "putuo",
                    "neighbors": []
                },
                "_friends": [],
                "secret": null
            }
        ]
    },
    "_friends": [
        {
            "relation": "my best friend",
            "secret": {
                "type": "rocker",
                "age": 18
            }
        },
        {
            "relation": "new friend",
            "secret": "little girl"
        },
        {
            "relation": "third friend",
            "secret": 3
        }
    ],
    "secret": "the kind!"
}

8 旧版序列化结果

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


