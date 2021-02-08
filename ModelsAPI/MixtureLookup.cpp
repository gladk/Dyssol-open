/* Copyright (c) 2020, Dyssol Development Team. All rights reserved. This file is part of Dyssol. See LICENSE file for license information. */

#include "MixtureLookup.h"
#include "ContainerFunctions.h"
#include <utility>

CMixtureLookup::CMixtureLookup(std::vector<CDependentValues> _components)
	: m_componets{ std::move(_components) }
{
	m_weights.resize(m_componets.size(), 1.0);
	Update();
}

CMixtureLookup::CMixtureLookup(std::vector<CDependentValues> _components, std::vector<double> _weights)
	: m_componets{ std::move(_components) }
	, m_weights{ std::move(_weights) }
{
	if (m_componets.size() == m_weights.size())
		Update();
	else
		Clear();
}

void CMixtureLookup::AddComponent(const CDependentValues& _component, double _weight)
{
	m_componets.push_back(_component);
	m_weights.push_back(_weight);
	Update();
}

void CMixtureLookup::RemoveComponent(size_t _index)
{
	if (_index >= m_componets.size()) return;
	m_componets.erase(m_componets.begin() + _index);
	m_weights.erase(m_weights.begin() + _index);
	Update();
}

size_t CMixtureLookup::ComponentsNumber() const
{
	return m_componets.size();
}

void CMixtureLookup::SetWeights(const std::vector<double>& _weights)
{
	if (_weights.size() != m_componets.size()) return;
	m_weights = _weights;
	Update();
}

std::vector<double> CMixtureLookup::GetWeights() const
{
	return m_weights;
}

double CMixtureLookup::GetLeft(double _right) const
{
	return m_table.GetLeft(_right);
}

double CMixtureLookup::GetRight(double _left) const
{
	return m_table.GetRight(_left);
}

void CMixtureLookup::Set(const CDependentValues& _component, double _weight)
{
	m_table.SetLeftToRight(_component);
	Multiply(_weight);
}

void CMixtureLookup::Add(double _value)
{
	m_table.Add(_value);
}

void CMixtureLookup::Add(const CDependentValues& _component, double _weight)
{
	if (m_table.IsEmpty())
		Set(_component, _weight);
	else
		m_table.AddMult(_component, _weight);
}

void CMixtureLookup::Add(const CMixtureLookup& _table, double _weight)
{
	if (_table.ComponentsNumber() != ComponentsNumber()) return;
	Add(_table.m_table.GetLeftToRightTable(), _weight);
}

void CMixtureLookup::Multiply(double _value)
{
	m_table.Mult(_value);
}

void CMixtureLookup::Clear()
{
	m_table.Clear();
	m_componets.clear();
	m_weights.clear();
}

void CMixtureLookup::Update()
{
	m_table.Clear();
	for (size_t i = 0; i < m_componets.size(); ++i)
		Add(m_componets[i], m_weights[i]);
}

void CMixtureLookup::CTwoWayMapExt::Add(double _value)
{
	for (auto& [l, r] : m_direct)
		r += _value;
	m_revert = Reverted(m_direct);
}

void CMixtureLookup::CTwoWayMapExt::Add(const CDependentValues& _table)
{
	const auto params = VectorsUnionUnsorted(m_direct.GetParamsList(), _table.GetParamsList());
	for (const auto& p : params)
		m_direct.SetValue(p, m_direct.GetValue(p) + _table.GetValue(p));
	m_revert = Reverted(m_direct);
}

void CMixtureLookup::CTwoWayMapExt::AddMult(const CDependentValues& _table, double _value)
{
	const auto params = VectorsUnionUnsorted(m_direct.GetParamsList(), _table.GetParamsList());
	for (const auto& p : params)
		m_direct.SetValue(p, m_direct.GetValue(p) + _table.GetValue(p) * _value);
	m_revert = Reverted(m_direct);
}

void CMixtureLookup::CTwoWayMapExt::Mult(double _value)
{
	for (auto& [l, r] : m_direct)
		r *= _value;
	m_revert = Reverted(m_direct);
}