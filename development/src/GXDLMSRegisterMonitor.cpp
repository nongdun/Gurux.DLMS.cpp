//
// --------------------------------------------------------------------------
//  Gurux Ltd
//
//
//
// Filename:        $HeadURL$
//
// Version:         $Revision$,
//                  $Date$
//                  $Author$
//
// Copyright (c) Gurux Ltd
//
//---------------------------------------------------------------------------
//
//  DESCRIPTION
//
// This file is a part of Gurux Device Framework.
//
// Gurux Device Framework is Open Source software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; version 2 of the License.
// Gurux Device Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General Public License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------

#include "../include/GXDLMSRegisterMonitor.h"
#include "../include/GXDLMSClient.h"

/**
 Constructor.
*/
CGXDLMSRegisterMonitor::CGXDLMSRegisterMonitor() : CGXDLMSObject(OBJECT_TYPE_REGISTER_MONITOR)
{
}

/**
 Constructor.

 @param ln Logical Name of the object.
*/
CGXDLMSRegisterMonitor::CGXDLMSRegisterMonitor(std::string ln) : CGXDLMSObject(OBJECT_TYPE_REGISTER_MONITOR, ln)
{
}

/**
 Constructor.

 @param ln Logical Name of the object.
 @param sn Short Name of the object.
*/
CGXDLMSRegisterMonitor::CGXDLMSRegisterMonitor(int sn) : CGXDLMSObject(OBJECT_TYPE_REGISTER_MONITOR, sn)
{
}

CGXDLMSRegisterMonitor::~CGXDLMSRegisterMonitor()
{
    m_Actions.clear();
    m_Thresholds.clear();
}
std::vector<CGXDLMSVariant> CGXDLMSRegisterMonitor::GetThresholds()
{
    return m_Thresholds;
}
void CGXDLMSRegisterMonitor::SetThresholds(std::vector<CGXDLMSVariant> value)
{
	m_Thresholds = value;
}

CGXDLMSMonitoredValue& CGXDLMSRegisterMonitor::GetMonitoredValue()
{
    return m_MonitoredValue;
}
void CGXDLMSRegisterMonitor::SetMonitoredValue(CGXDLMSMonitoredValue& value)
{
    m_MonitoredValue = value;
}

std::vector<CGXDLMSActionSet>& CGXDLMSRegisterMonitor::GetActions()
{
    return m_Actions;
}
void CGXDLMSRegisterMonitor::SetActions(std::vector<CGXDLMSActionSet>& value)
{
    m_Actions = value;
}


// Returns amount of attributes.
int CGXDLMSRegisterMonitor::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int CGXDLMSRegisterMonitor::GetMethodCount()
{
    return 0;
}

void CGXDLMSRegisterMonitor::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for(std::vector<CGXDLMSVariant>::iterator it = m_Thresholds.begin(); it != m_Thresholds.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = it->ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
    values.push_back(m_MonitoredValue.ToString());

    //Clear str.
    sb.str(std::string());
    sb << '[';
    empty = true;
    for(std::vector<CGXDLMSActionSet>::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = it->ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
}

void CGXDLMSRegisterMonitor::GetAttributeIndexToRead(std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Thresholds
    if (!IsRead(2))
    {
        attributes.push_back(2);
    }
    //MonitoredValue
    if (!IsRead(3))
    {
        attributes.push_back(3);
    }
    //Actions
    if (!IsRead(4))
    {
        attributes.push_back(4);
    }
}

int CGXDLMSRegisterMonitor::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return ERROR_CODES_OK;
    }
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return ERROR_CODES_OK;
    }
    if (index == 3)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return ERROR_CODES_OK;
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return ERROR_CODES_OK;
    }
    return ERROR_CODES_INVALID_PARAMETER;
}

/*
 * Returns value of given attribute.
 */
int CGXDLMSRegisterMonitor::GetValue(int index, int selector, CGXDLMSVariant& parameters, CGXDLMSVariant& value)
{
    CGXByteBuffer data;
    CGXDLMSVariant ln;
    if (index == 1)
    {
        return GetLogicalName(this, value);
    }
    if (index == 2)
    {
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        GXHelpers::SetObjectCount(m_Thresholds.size(), data);
        int ret;
        CGXDLMSVariant tmp;
        for(std::vector<CGXDLMSVariant>::iterator it = m_Thresholds.begin(); it != m_Thresholds.end(); ++it)
        {
            tmp = *it;
            if ((ret = GXHelpers::SetData(data, it->vt, tmp)) != 0)
            {
                return ret;
            }
        }
        value = data;
        return ERROR_CODES_OK;
    }
    if (index == 3)
    {
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(3);
        int ret;
        CGXDLMSVariant type = m_MonitoredValue.GetObjectType();
        ln = m_MonitoredValue.GetLogicalName();
        CGXDLMSVariant index = m_MonitoredValue.GetAttributeIndex();
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, type )) != ERROR_CODES_OK ||  //ClassID
			(ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != ERROR_CODES_OK || //LN
			(ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, index)) != ERROR_CODES_OK)
        {
            return ret;
        }
        value = data;
        return ERROR_CODES_OK;
    }
    if (index == 4)
    {
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        int ret;
        //Add count
        GXHelpers::SetObjectCount(m_Actions.size(), data);
        CGXDLMSVariant selector;
        for(std::vector<CGXDLMSActionSet>::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2);
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2);
            ln = it->GetActionDown().GetLogicalName();
            selector = it->GetActionDown().GetScriptSelector();
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != ERROR_CODES_OK ||
                    (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, selector)) != ERROR_CODES_OK)
            {
                return ret;
            }
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2);
            ln = it->GetActionUp().GetLogicalName();
            selector = it->GetActionUp().GetScriptSelector();
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != ERROR_CODES_OK ||
				(ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, selector)) != ERROR_CODES_OK)
            {
                return ret;
            }
        }
        value = data;
        return ERROR_CODES_OK;
    }
    return ERROR_CODES_INVALID_PARAMETER;
}

/*
 * Set value of given attribute.
 */
int CGXDLMSRegisterMonitor::SetValue(CGXDLMSSettings* settings, int index, CGXDLMSVariant& value)
{
    if (index == 1)
    {
        return SetLogicalName(this, value);
    }
    if (index == 2)
    {
        SetThresholds(value.Arr);
        return ERROR_CODES_OK;
    }
    if (index == 3)
    {
        GetMonitoredValue().SetObjectType((OBJECT_TYPE) value.Arr[0].ToInteger());
        std::string ln;
        GXHelpers::GetLogicalName(value.Arr[1].byteArr, ln);
        m_MonitoredValue.SetLogicalName(ln);
        m_MonitoredValue.SetAttributeIndex(value.Arr[2].ToInteger());
        return ERROR_CODES_OK;
    }
    if (index == 4)
    {
        m_Actions.clear();
        for (std::vector<CGXDLMSVariant>::iterator action_set = value.Arr.begin(); action_set != value.Arr.end(); ++action_set)
        {
            CGXDLMSActionSet set;
            CGXDLMSVariant& up = action_set->Arr[0];
            CGXDLMSVariant tmp;
            CGXDLMSClient::ChangeType(up.Arr[0], DLMS_DATA_TYPE_OCTET_STRING, tmp);
            set.GetActionUp().SetLogicalName(tmp.strVal);
            set.GetActionUp().SetScriptSelector(up.Arr[1].lVal);
            CGXDLMSVariant& down = action_set->Arr[1];
            CGXDLMSClient::ChangeType(down.Arr[0], DLMS_DATA_TYPE_OCTET_STRING, tmp);
            set.GetActionDown().SetLogicalName(tmp.strVal);
            set.GetActionDown().SetScriptSelector(down.Arr[1].lVal);
            m_Actions.push_back(set);
        }
        return ERROR_CODES_OK;
    }
    return ERROR_CODES_INVALID_PARAMETER;
}