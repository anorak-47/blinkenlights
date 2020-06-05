#include "keyboard_interface.h"
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QDebug>

KeyboardInterface::KeyboardInterface()
{
}

KeyboardInterface::~KeyboardInterface()
{
}

QString KeyboardInterface::getColorMapAsJSONString(std::unique_ptr<s_map> colormap, bool compact)
{
    unsigned int mapid = colormap->map;

    qDebug() << "colormap: map " << mapid;

    QJsonObject jsmap;

    jsmap["map"] = (int)mapid;

    QJsonArray jsrows;
    for (unsigned int row = 0; row < colormap->rows.size(); ++row)
    {
        struct s_map_row const &map_row = colormap->rows[row];

        if (map_row.entries.empty())
        {
            qDebug() << __PRETTY_FUNCTION__ << ": ERROR: row without entries!";
            return QString();
        }

        QJsonObject jsrow;
        jsrow["row"] = (int)map_row.row;

        QJsonArray jscols;
        for (auto &key : map_row.entries)
        {
            QJsonArray jscol;
            jscol.push_back((int)key.col);
            jscol.push_back(key.color.name().remove(0,1));
            jscols.push_back(jscol);
        }

        jsrow["cols"] = jscols;
        jsrows.push_back(jsrow);
    }

    jsmap["rows"] = jsrows;

    QJsonDocument jd(jsmap);
    return jd.toJson(compact ? QJsonDocument::Compact : QJsonDocument::Indented);
}

sMapPtr KeyboardInterface::getColorMapFromJSONString(QString jsonstring)
{
    sMapPtr map = std::shared_ptr<s_map>(new s_map);

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonstring.toUtf8(), &error);

    if (!doc.isNull() && doc.isObject())
    {
        QJsonObject obj = doc.object();

        //qDebug() << __PRETTY_FUNCTION__ << ": obj";

        QJsonValue jmap = obj.value("map");
        if (jmap.isNull() || !jmap.isDouble())
        {
            qDebug() << "key 'map' not found [" << jsonstring << "]";
            return map;
        }

        //qDebug() << __PRETTY_FUNCTION__ << ": map " << jmap.toDouble();

        map->map = jmap.toInt();

        QJsonValue jrows = obj.value("rows");
        if (jrows.isNull() || !jrows.isArray())
        {
            qDebug() << "key 'rows' not found [" << jsonstring << "]";
            return map;
        }

        QJsonArray jarows = jrows.toArray();

        for (auto const &row : jarows)
        {
            s_map_row map_row;
            QJsonObject jarow = row.toObject();

            QJsonValue jrowid = jarow.value("row");
            if (jrowid.isNull() || !jrowid.isDouble())
            {
                qDebug() << "key 'row' not found [" << jsonstring << "]";
                return map;
            }

            unsigned int response_row = jrowid.toInt();
            map_row.row = response_row;

            QJsonValue jcols = jarow.value("cols");
            if (jcols.isNull() || !jcols.isArray())
            {
                qDebug() << "key 'cols' not found [" << jsonstring << "]";
                return map;
            }

            QJsonArray jacols = jcols.toArray();

            //qDebug() << __PRETTY_FUNCTION__ << ": cols " << jacols.size();

            for (auto const &col : jacols)
            {
                QJsonArray jacol = col.toArray();

                //qDebug() << __PRETTY_FUNCTION__ << ": col size " << jacol.size();

                unsigned int response_col = jacol[0].toDouble();
                QColor response_color(QString("#%1").arg(jacol[1].toString()));

                //qDebug() << __PRETTY_FUNCTION__ << ": col " << response_col;
                //qDebug() << __PRETTY_FUNCTION__ << ": color " << response_color.name();

                map_row.entries.emplace_back(s_map_entry(response_row, response_col, response_color));
            }

            map->rows.push_back(map_row);
        }

        //qDebug() << __PRETTY_FUNCTION__ << ": got row '" << response_row << "' with " << map_row.entries.size() << " column entries";
    }
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << ": ERROR: parsing the json response string: <" << doc.toJson() << ">";
        qDebug() << __PRETTY_FUNCTION__ << ": ERROR: " << error.errorString();
    }

    qDebug() << "map: " << map->map;
    qDebug() << "map rows: " << map->rows.size();

    return map;
}
