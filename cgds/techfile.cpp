#include <cassert>
#include <cstdlib>
#include "techfile.h"

namespace GDS
{

DisplayLayerNode::DisplayLayerNode(std::string layer_name, std::string purpose_name, std::string stipple_name)
    :layer_name_(layer_name), purpose_name_(purpose_name), stipple_name_(stipple_name)
{
    r_ = rand() % 256;
    g_ = rand() % 256;
    b_ = rand() % 256;
}

Techfile& Techfile::instance()
{
    static Techfile inst;
    return inst;
}

bool Techfile::AddLayer(short layer, short purpose,
                        std::string layer_name, std::string purpose_name,
                        std::string packet_name)
{
    // If the new layer has existed.
    assert(layer >= 0 && layer <= 255);
    assert(purpose >= 0 && layer <= 255);

    bool layer_exist = (ParserLayer(layer) != "");
    bool purpose_exist = (ParserPurpose(layer) != "");
    // check the layer_name is existed or not;
    if (!layer_exist && layers_.find(layer_name) != layers_.end())
        return false;
    if (!purpose_exist && purpose_.find(purpose_name) != purpose_.end())
        return false;
    if (layer_exist && layer_name != ParserLayer(layer))
        ChangeLayerName(layer, layer_name);
    if (purpose_exist && purpose_name != ParserPurpose(purpose))
        ChangePurposeName(purpose, purpose_name);

    if (!layer_exist)
    {
        layer_name = (layer_name == "") ? "L" + std::to_string(layer) : layer_name;
        layers_.insert(std::make_pair(layer_name, layer));
    }
    if (!purpose_exist)
    {
        purpose_name = (purpose_name == "") ? "P" + std::to_string(purpose) : purpose_name;
        purpose_.insert(std::make_pair(purpose_name, purpose));
    }

    bool display_exist = false;
    for (auto node : display_priority_)
    {
        if (ParserLayer(layer) == node.layer_name()
                && ParserPurpose(purpose) == node.purpose_name())
        {
            display_exist = true;
            break;
        }
    }
    if (!display_exist)
        display_priority_.push_back(DisplayLayerNode(ParserLayer(layer), ParserPurpose(purpose), packet_name));

    return true;
}

bool Techfile::GetLayerColor(short layer, short purpose, char &r, char &g, char &b) const
{
    std::string layer_name = ParserLayer(layer);
    std::string purpose_name = ParserLayer(purpose);
    if (layer_name == "" || purpose_name == "")
        return false;

    bool existed = false;
    for (auto n : display_priority_)
    {
        if (n.layer_name() == layer_name && n.purpose_name() == purpose_name)
        {
            existed = true;
            n.color(r, g, b);
            break;
        }
    }
    return existed;
}

bool Techfile::GetLayerStippleName(short layer, short purpose, std::string &stipple_name) const
{
    std::string layer_name = ParserLayer(layer);
    std::string purpose_name = ParserLayer(purpose);
    if (layer_name == "" || purpose_name == "")
        return false;

    bool existed = false;
    for (auto n : display_priority_)
    {
        if (n.layer_name() == layer_name && n.purpose_name() == purpose_name)
        {
            existed = true;
            stipple_name = n.stipple_name();
            break;
        }
    }
    return existed;
}

bool Techfile::GetLayerStipplePattern(short layer, short purpose, StipplePattern &pattern) const
{
    std::string layer_name = ParserLayer(layer);
    std::string purpose_name = ParserLayer(purpose);
    if (layer_name == "" || purpose_name == "")
        return false;

    bool existed = false;
    for (auto n : display_priority_)
    {
        if (n.layer_name() == layer_name && n.purpose_name() == purpose_name)
        {
            existed = true;
            pattern = n.stipple_pattern();
            break;
        }
    }
    return existed;
}

std::string Techfile::ParserLayer(short layer) const
{
    std::string ret = "";
    for (auto iter = layers_.begin(); iter != layers_.end(); iter++)
    {
        if (iter->second == layer)
        {
            ret = iter->first;
            break;
        }
    }
    return ret;
}

std::string Techfile::ParserPurpose(short purpose) const
{
    std::string ret = "";
    for (auto iter = purpose_.begin(); iter != purpose_.end(); iter++)
    {
        if (iter->second == purpose)
        {
            ret = iter->first;
            break;
        }
    }
    return ret;
}

void Techfile::ChangeLayerName(short layer, std::string name)
{
    std::string cur_name = ParserLayer(layer);
    if (cur_name == "")
        return;
    for (size_t i = 0; i < display_priority_.size(); i++)
    {
        if (display_priority_[i].layer_name() == cur_name)
            display_priority_[i].set_layer_name(name);
    }
    auto iter = layers_.find(cur_name);
    layers_.erase(iter);
    layers_[name] = layer;
}

void Techfile::ChangePurposeName(short purpose, std::string name)
{
    std::string cur_name = ParserPurpose(purpose);
    if (cur_name == "")
        return;
    for (size_t i = 0; i < display_priority_.size(); i++)
    {
        if (display_priority_[i].purpose_name() == cur_name)
            display_priority_[i].set_purpose_name(name);
    }
    auto iter = purpose_.find(cur_name);
    purpose_.erase(iter);
    purpose_[name] = purpose;
}

}
