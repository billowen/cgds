#ifndef GDS_TECHFILE_H
#define GDS_TECHFILE_H
#include <vector>
#include <map>
#include <string>

namespace GDS
{

typedef std::vector<std::vector<char> > StipplePattern;

// These are the build brush type of Qt.
static std::map<std::string, int> kBuildInStipple =
{
    { "NoBrush", 0 },
    { "SolidPattern", 1 },
    { "Dense1Pattern", 2 },
    { "Dense2Pattern", 3 },
    { "Dense3Pattern", 4 },
    { "Dense4Pattern", 5 },
    { "Dense5Pattern", 6 },
    { "Dense6Pattern", 7 },
    { "Dense7Pattern", 8 },
    { "HorPattern", 9 },
    { "VerPattern", 10 },
    { "CrossPattern", 11 },
    { "BDiagPattern", 12 },
    { "FDiagPattern", 13 },
    { "DiagCrossPattern", 14 }
};

class DisplayLayerNode
{
public:
    DisplayLayerNode(std::string layer_name, std::string purpose_name, std::string stipple_name = "NoBrush");
    ~DisplayLayerNode() {}

    std::string layer_name() const { return layer_name_; }
    std::string purpose_name() const { return purpose_name_; }
    std::string packet_name() const { return packet_name_; }
    void color(char &r, char &g, char &b) const { r = r_; g = g_; b = b_; }
    std::string stipple_name() const { return stipple_name_; }
    StipplePattern stipple_pattern() const { return stipple_pattern; }

    void set_layer_name(std::string name) { layer_name_ = name; }
    void set_purpose_name(std::string name) { purpose_name_ = name; }
    void set_packet_name(std::string name) { packet_name_ = name; }

private:
    std::string layer_name_;
    std::string purpose_name_;
    // If the stipple_pattern is empty, use the build in pattern;
    std::string stipple_name_;
    StipplePattern stipple_pattern_;
    char r_, g_, b_;
};

class Techfile
{
public:
    Techfile() {}
    ~Techfile() {}

    static Techfile& instance();
    bool AddLayer(short layer, short purpose,
                  std::string layer_name = "", std::string purpose_name = "",
                  std::string packet_name = "NoBrush");
    bool GetLayerColor(short layer, short purpose, char &r, char &g, char &b) const;
    bool GetLayerStippleName(short layer, short purpose, std::string &stipple_name) const;
    bool GetLayerStipplePattern(short layer, short purpose, StipplePattern &pattern) const;

private:
    /**
     * @brief Parser the layer/purpose number into string
     * @param layer Layer number.
     * @return The layer/purpose name or empty string if the layer is not existed.
     */
    std::string ParserLayer(short layer) const;
    std::string ParserPurpose(short purpose) const;
    /**
     * @brief Change the layer name of specified layer.
     * @param layer The layer number of specified layer.
     * @param name The new name of specified layer.
     */
    void ChangeLayerName(short layer, std::string name);
    /**
     * @brief Change the purpose name of specified purpose.
     * @param purpose The purpose number of specified purpose.
     * @param name The nwe name of specified purpose.
     */
    void ChangePurposeName(short purpose, std::string name);

    std::map<std::string, short> layers_;
    std::map<std::string, short> purpose_;
    std::vector<DisplayLayerNode> display_priority_;
};

}



#endif
