#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include "ExpPublic.h"

namespace fs = boost::filesystem;

ExpDiaDBHandler diadb_load(void) {
    ExpBool memMapSw = EXP_FALSE;

    // ダイヤDB生成
    const fs::path dia_d_path("data/dia");
    ExpDiaDataFileList dia_entry_list;
    dia_entry_list = ExpDiaDB_NewFileList();
    BOOST_FOREACH(const fs::path& p, std::make_pair(fs::directory_iterator(dia_d_path),
                                                    fs::directory_iterator())) {
        if (!fs::is_directory(p)) {  
            ExpDiaDB_AddFileList3(p.c_str(), EXP_IO_LEVEL_HIGH, memMapSw, dia_entry_list);
        }
    }
    ExpErr err;
    ExpDiaDBHandler db =ExpDiaDB_Initiate( dia_entry_list, 0, &err);
    ExpDiaDB_DeleteFileList(dia_entry_list);
    return db;
}

ExpDataHandler data_load(ExpDiaDBHandler diadb_h) {
    ExpBool memMapSw = EXP_FALSE;

    // DB生成
    ExpBool load_english_data = EXP_FALSE;
    ExpFileID   dummy_file_id;
    const fs::path knb_d_path("data/knb");
    ExpDataFileList knb_entry_list = ExpDB_NewFileList();
    BOOST_FOREACH(const fs::path& p, std::make_pair(fs::directory_iterator(knb_d_path),
                                                    fs::directory_iterator())) {
        if (!fs::is_directory(p)) {
            ExpDB_AddFileList3( p.c_str(), EXP_IO_LEVEL_HIGH, memMapSw, knb_entry_list, dummy_file_id);
        }
    }
    ExpErr err;
    ExpDataHandler db = ExpDB_Initiate( knb_entry_list, diadb_h, load_english_data, &err);
    ExpDB_DeleteFileList(knb_entry_list);
    return db;
}


void output_result(ExpRouteResHandler searchResult) {
    int routeCount = ExpRoute_GetRouteCount(searchResult);
    ExpChar name[512];
    for(int routeNo = 1; routeNo<=routeCount; routeNo++) {
        int railCount = ExpCRoute_GetRailCount(searchResult, routeNo);
        std::cout << "route" << routeNo << "================================" << std::endl;
        for(int railNo = 1; railNo<=railCount; railNo++) {
            ExpCRouteRPart_GetStationName( searchResult, routeNo, railNo, EXP_LANG_JAPANESE, name, 512, EXP_FALSE);
            std::cout << name << std::endl;
            ExpCRouteRPart_GetRailName( searchResult, routeNo, railNo, EXP_LANG_JAPANESE, name, 512, EXP_FALSE);
            std::cout << name << std::endl;
        }
        ExpCRouteRPart_GetStationName( searchResult, routeNo, railCount+1, EXP_LANG_JAPANESE, name, 512, EXP_FALSE);
        std::cout << name << std::endl;
    }
}

int main() {
    ExpDiaDBHandler diadb_h = diadb_load();
    ExpDataHandler db_handler = data_load(diadb_h);

    // 探索
    ExpErr err;
    ExpNaviHandler   navi_handler   = ExpNavi_NewHandler(db_handler, &err);
    ExpStationCode code1;
    ExpStationCode code2;
    ExpStation_SetEmptyCode(&code1);
    ExpStation_SetEmptyCode(&code2);
    ExpStation_SharedCodeToCode( db_handler, 22828, &code1);
    ExpStation_SharedCodeToCode( db_handler, 25853, &code2);
    ExpNavi_SetStationEntry( navi_handler, 1, &code1 );
    ExpNavi_SetStationEntry( navi_handler, 2, &code2 );

    //出発
    ExpInt16 searchMode = 0;
    ExpDate  searchDate = 20150625;
    ExpInt16 searchTime = 600;
    ExpDiaVehicles vehicles;
    ExpDiaVehicles_Clear(&vehicles, EXP_TRUE);
    ExpRouteResHandler searchResult = ExpRoute_DiaSearch(  navi_handler, 
                                                            searchMode,
                                                            searchDate,
                                                            searchTime,
                                                            nullptr, 
                                                            0,
                                                            &vehicles );


    output_result(searchResult);

    ExpNavi_DeleteHandler(navi_handler);
    ExpRoute_Delete(searchResult);
    ExpDB_Terminate(db_handler);
    ExpDiaDB_Terminate(diadb_h);
}
