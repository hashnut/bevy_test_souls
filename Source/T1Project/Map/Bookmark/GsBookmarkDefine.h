#pragma once
/*
    북마크 정렬 타입
*/
enum class EGsBookmarkSortType :uint8
{
    Name,               // 이름순
    EnrollTime,         // 등록 시간순
};
// 북마트 ui 타입
enum class EGsBookmarkUIType : uint8
{
    Bookmark,           // 북마크
    RegionMap,          // 지역맵
};