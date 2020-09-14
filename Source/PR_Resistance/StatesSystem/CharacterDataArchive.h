// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PR_RESISTANCE_API CharacterDataArchive
{
private:
	TMap<FName,void*> mDatas;
public:
	CharacterDataArchive();
	~CharacterDataArchive();

	/*
	* clear data map
	*/
	void Init();
	/*
	* key : �����͸� ã���� ���� Ű��
	* data : ������ ������, �������� �����ֱ�� AddData�� ���� ��ü�� å������.
	*/
	bool AddData(FName key, void* data);
	/*
	* Ű�� �ش��ϴ� �����͸� Archive���� �����Ѵ�.
	*/
	void RemoveData(FName key);
	/*
	*  data : �� ���޿� ������
	*  return : false �� ã�� ����, true ã�� ����
	*/
	bool GetData(FName key, _Out_ void** data);
};
