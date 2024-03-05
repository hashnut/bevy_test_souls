// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * Hive IAPv4 기본 인터페이스
 */
class IGsIAP
{
public:
	virtual ~IGsIAP() {}

public:
	virtual void Initialize() {};
	virtual void TryPurchase(const uint16 InProductID, const uint16 InAmount) {};

};
