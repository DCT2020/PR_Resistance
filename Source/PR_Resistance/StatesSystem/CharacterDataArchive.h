// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterDataArchive.generated.h"
/**
 * 
 */
 UCLASS()
class PR_RESISTANCE_API UCharacterDataArchive : public UObject
{
	GENERATED_BODY()
private:
	TMap<FName,void*> mDatas;
public:
	UCharacterDataArchive();
	~UCharacterDataArchive();

	/*
	* clear data map
	*/
	void Init();
	/*
	* key : 데이터를 찾을때 쓰는 키값
	* data : 공유할 데이터, 데이터의 생명주기는 AddData를 콜한 객체가 책임진다.
	*/
	bool AddData(FName key, void* data);
	/*
	* 키에 해당하는 데이터를 Archive에서 제외한다.
	*/
	void RemoveData(FName key);
	/*
	*  data : 값 전달용 포인터
	*  return : false 면 찾기 실패, true 찾기 성공
	*/
	bool GetData(FName key, _Out_ void** data);
};
