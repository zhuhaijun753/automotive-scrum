//~~ void create_artefact_statechart(std::ofstream& outfile, const std::string& outpath) [artefact_base] ~~
if (state_changes.empty())
    return;

const I_element* e = dynamic_cast<const I_element*>(this);

FILE *gfile;
std::string linkname = e->getFullIdentifier() + "_artefact_states";
std::string filename = outpath + "/" + linkname;

std::string command("dot ");
command += "-Tpng -o " + filename + ".png ";
command += "-Tcmapx -o " + filename + ".cmapx";

std::set<std::string> nodes;
for (std::vector<artefact_transition>::const_iterator it = state_changes.begin(); it != state_changes.end(); it++)
{
    nodes.insert((*it).getSource());
    nodes.insert((*it).getDestination());
}

if(!(gfile = popen(command.c_str(), "w")))
    throw(command);

fputs("digraph artefact_states {\n", gfile);
//fputs("rankdir=\"LR\";\n", gfile);

for (std::set<std::string>::iterator it = nodes.begin(); it != nodes.end(); it++)
{
    std::string node = (*it) + " [";
    

    if ((*it)=="null")
        node += "shape=circle width=0.2, label=\"\"";
    else
        node = node + "shape=Mrecord, label=\"{" 
             + breakApart(label_owner::getLabel())
             + "|"
             + (*it) + "}\", ";

    node += std::string("style=filled, color=\"#AFCEBE\", fillcolor=\"#177445\", ") + graphics_creator::text_standards
         += "fontcolor=\"#AFCEBE\"];";

    fputs(node.c_str(),gfile);
}

for (std::vector<artefact_transition>::const_iterator it = state_changes.begin(); it != state_changes.end(); it++)
{
    if ((*it).getDestination() != "null")
    {
        std::string connection;
        connection = (*it).getSource()
            + " -> "
            + (*it).getDestination()
            + "["
            +  text_standards
            + "tooltip=\""
            + (*it).getElement()->getBrief()
            + "\", label=\""
            + (*it).getElement()->breakApart((*it).getElement()->getLabel());
            if (!(*it).getGuard().empty())
                connection += std::string("\\n[") + (*it).getGuard() + "]";
            connection += "\", URL=\""
            + (*it).getElement()->getHtmlFilename()
            + "\", fontcolor=\"#177445\", color=\"#177445 \"];";
        
        fputs(connection.c_str(),gfile);
    }
}


fputs("}\n",gfile);

create_headline(outfile, "Artefact state chart");

outfile << "<IMG SRC=\""
        << linkname
        << ".png\" USEMAP=\"#artefact_states\" title=\"Artefact state chart of artefact: "
        << e->getLabel()
        << "\" alt=\"state diagram of artefact: "
        << e->getLabel()
        << "\" />"
        << std::endl;

pclose(gfile);
insert_file(outfile, filename + ".cmapx");
